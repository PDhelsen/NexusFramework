#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/File.h"

#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/Directory.h"

namespace NxFr
{
	Platform::FileMode ConvertFileToPlatformMode(File::Mode Mode)
	{
		switch (Mode)
		{
		case File::Mode::None: return Platform::FileMode::None;
		case File::Mode::Read: return Platform::FileMode::Read;
		case File::Mode::Write: return Platform::FileMode::Write;
		case File::Mode::Append: return Platform::FileMode::Append;
		}

		return Platform::FileMode::Read;
	}

	File::File(StringView Path)
		: Path(), FileMode(File::Mode::None), Handle(nullptr)
	{
		SetPath(Path);
	}

	File::File(File&& Other) noexcept
		: Path(::NxFr::Move(Other.Path)), FileMode(Other.FileMode), Handle(Other.Handle)
	{
		Other.Path = "";
		Other.FileMode = File::Mode::None;
		Other.Handle = nullptr;
	}

	File::~File()
	{
		NX_ASSERT(!Handle, Default, "Delete file while it is still open: %s", Path.C());
	}

	File& File::operator=(File&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Path = ::NxFr::Move(Other.Path);
		FileMode = Other.FileMode;
		Handle = Other.Handle;

		Other.FileMode = File::Mode::None;
		Other.Handle = nullptr;

		return *this;
	}

	File::operator bool() const
	{
		return Exists();
	}

	bool File::operator==(const File& Other) const
	{
		return Path == Other.Path;
	}

	bool File::operator!=(const File& Other) const
	{
		return !(*this == Other);
	}

	bool File::Exists() const
	{
		return Path::Exist(Path);
	}

	void File::EnsureParent() const
	{
		Path::EnsureParent(Path);
	}

	void File::Create(bool KeepOpen)
	{
		if (Path.IsEmpty())
		{
			NX_LOG(Error, Default, "Impossible to create file with empty path");
			return;
		}

		if (Exists())
		{
			if (KeepOpen)
			{
				Open(Mode::Write);
				return;
			}
			else
			{
				return;
			}
		}

		NX_ASSERT(!Exists() && !Handle, Default, "Failed to create file: %s", Path.C());

		Path::EnsureParent(Path);
		Handle = Globals::PlatformTarget->FileCreate(Path, KeepOpen);
		FileMode = File::Mode::Write;

		NX_ASSERT(Exists() && ((KeepOpen && Handle) || (!KeepOpen && !Handle)), Default, "Failed to create file: %s", Path.C());
	}

	void File::Move(StringView Target, bool Override, bool CloseIfOpen)
	{
		if (Path.IsEmpty() || Target.IsEmpty() || Path == Target)
		{
			NX_LOG(Error, Default, "Impossible to move file with empty path");
			return;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NX_ASSERT(Exists() && !Handle && Path != Target && !Path::Exist(Target), Default, "Failed to move file: %s", Path.C());

		Path::EnsureParent(Target);
		Globals::PlatformTarget->FileMove(Path, Target, Override);
		FileMode = File::Mode::None;
		SetPath(Target);

		NX_ASSERT(Exists() && !Handle, Default, "Failed to move file: %s", Path.C());
	}

	void File::Copy(StringView Target, bool Override, bool CloseIfOpen)
	{
		if (Path.IsEmpty() || Target.IsEmpty() || Path == Target)
		{
			NX_LOG(Error, Default, "Impossible to copy file with empty path");
			return;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NX_ASSERT(Exists() && !Handle && Path != Target && !Path::Exist(Target), Default, "Failed to copy file: %s", Path.C());

		Path::EnsureParent(Target);
		Globals::PlatformTarget->FileCopy(Path, Target, Override);
		FileMode = File::Mode::None;

		NX_ASSERT(Exists() && !Handle, Default, "Failed to copy file: %s", Path.C());
	}

	void File::Delete(bool CloseIfOpen)
	{
		if (Path.IsEmpty())
		{
			NX_LOG(Error, Default, "Impossible to delete file with empty path");
			return;
		}

		if (!Exists())
		{
			return;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NX_ASSERT(Exists() && !Handle, Default, "Failed to delete file: %s", Path.C());

		Globals::PlatformTarget->FileDelete(Path);
		FileMode = File::Mode::None;

		NX_ASSERT(!Exists() && !Handle, Default, "Failed to delete file: %s", Path.C());
	}

	void File::Open(Mode OpenMode, bool CreateIfDontExist)
	{
		if (Path.IsEmpty())
		{
			NX_LOG(Error, Default, "Impossible to open file with empty path");
			return;
		}

		if (Handle)
		{
			return;
		}

		if (CreateIfDontExist && !Exists())
		{
			return Create(true);
		}

		NX_ASSERT(Exists() && !Handle, Default, "Failed to open file: %s", Path.C());

		Handle = Globals::PlatformTarget->FileOpen(Path, ConvertFileToPlatformMode(OpenMode));
		FileMode = OpenMode;

		NX_ASSERT(Handle, Default, "Failed to open file: %s", Path.C());
	}

	void File::Close()
	{
		if (Path.IsEmpty())
		{
			NX_LOG(Error, Default, "Impossible to close file with empty path");
			return;
		}

		if (!Handle)
		{
			return;
		}

		NX_ASSERT(Exists() && Handle, Default, "Failed to close file: %s", Path.C());

		Globals::PlatformTarget->FileClose(Handle);
		FileMode = File::Mode::None;
		Handle = nullptr;

		NX_ASSERT(Exists() && !Handle, Default, "Failed to close file: %s", Path.C());
	}

	uint64 File::GetSize() const
	{
		NX_ASSERT(Exists() && Handle, Default, "Failed to query file size: %s", Path.C());

		return Globals::PlatformTarget->FileSize(Handle);
	}

	void File::WriteByte(BufferView Data)
	{
		NX_ASSERT(Exists() && Handle && Data.GetPtr(), Default, "Failed to write file: %s", Path.C());

		Globals::PlatformTarget->FileWriteByte(Handle, Data);
	}

	Buffer File::ReadByte() const
	{
		NX_ASSERT(Exists() && Handle, Default, "Failed to read file: %s", Path.C());

		return Globals::PlatformTarget->FileReadByte(Handle);
	}

	void File::WriteText(StringView Text)
	{
		NX_ASSERT(Exists() && Handle && Text.C(), Default, "Failed to write file: %s", Path.C());

		Globals::PlatformTarget->FileWriteText(Handle, Text);
	}

	String File::ReadText() const
	{
		NX_ASSERT(Exists() && Handle, Default, "Failed to read file: %s", Path.C());

		return Globals::PlatformTarget->FileReadText(Handle);
	}

	void File::SetPath(StringView Value)
	{
		Path = Value;
		Path::Normalize(Path);
	}
}
