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
		case NxFr::File::Mode::None: return Platform::FileMode::None;
		case NxFr::File::Mode::Read: return Platform::FileMode::Read;
		case NxFr::File::Mode::Write: return Platform::FileMode::Write;
		case NxFr::File::Mode::Append: return Platform::FileMode::Append;
		}

		return Platform::FileMode::Read;
	}

	File::File(StringView Path)
		: Path(), Exist(false), FileMode(File::Mode::None), Handle(nullptr)
	{
		SetPath(Path);
		Refresh();
	}

	File::File(File&& Other) noexcept
		: Path(Other.Path), Exist(Other.Exist), FileMode(Other.FileMode), Handle(Other.Handle)
	{
		Other.Handle = nullptr;
	}

	File::~File()
	{
		NEXUS_ASSERT(!Handle, Default, "Delete file while it is still open: %s", Path.C());
	}

	File& File::operator=(File&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Path = Other.Path;
		Exist = Other.Exist;
		FileMode = Other.FileMode;
		Handle = Other.Handle;

		Other.Handle = nullptr;

		return *this;
	}

	File::operator bool() const
	{
		return Exist;
	}

	bool File::operator==(const File& Other) const
	{
		return Path == Other.Path;
	}

	bool File::operator!=(const File& Other) const
	{
		return !(*this == Other);
	}

	File& File::Refresh()
	{
		Exist = Path::Exist(Path);
		return *this;
	}

	File& File::EnsureParent()
	{
		Path::EnsureParent(Path);
		return *this;
	}

	void File::Create(bool KeepOpen)
	{
		if (Exist)
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

		NEXUS_ASSERT(!Exist && !Handle, Default, "Failed to create file: %s", Path.C());

		Path::EnsureParent(Path);
		Handle = Platform::GetInstance()->FileCreate(Path, KeepOpen);
		FileMode = File::Mode::Write;
		Refresh();

		NEXUS_ASSERT(Exist && ((KeepOpen && Handle) || (!KeepOpen && !Handle)), Default, "Failed to create file: %s", Path.C());
	}

	void File::Move(StringView Target, bool Override, bool CloseIfOpen)
	{
		if (Path == Target)
		{
			return;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NEXUS_ASSERT(Exist && !Handle && Path != Target && !Path::Exist(Target), Default, "Failed to move file: %s", Path.C());

		Path::EnsureParent(Target);
		Platform::GetInstance()->FileMove(Path, Target, Override);
		FileMode = File::Mode::None;
		SetPath(Target);
		Refresh();

		NEXUS_ASSERT(Exist && !Handle, Default, "Failed to move file: %s", Path.C());
	}

	void File::Copy(StringView Target, bool Override, bool CloseIfOpen)
	{
		if (Path == Target)
		{
			return;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NEXUS_ASSERT(Exist && !Handle && Path != Target && !Path::Exist(Target), Default, "Failed to copy file: %s", Path.C());

		Path::EnsureParent(Target);
		Platform::GetInstance()->FileCopy(Path, Target, Override);
		FileMode = File::Mode::None;
		Refresh();

		NEXUS_ASSERT(Exist && !Handle, Default, "Failed to copy file: %s", Path.C());
	}

	void File::Delete(bool CloseIfOpen)
	{
		if (!Exist)
		{
			return;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NEXUS_ASSERT(Exist && !Handle, Default, "Failed to delete file: %s", Path.C());

		Platform::GetInstance()->FileDelete(Path);
		FileMode = File::Mode::None;
		Refresh();

		NEXUS_ASSERT(!Exist && !Handle, Default, "Failed to delete file: %s", Path.C());
	}

	void File::Open(Mode OpenMode, bool CreateIfDontExist)
	{
		if (Handle)
		{
			return;
		}

		if (CreateIfDontExist && !Exist)
		{
			return Create(true);
		}

		NEXUS_ASSERT(Exist && !Handle, Default, "Failed to open file: %s", Path.C());

		Handle = Platform::GetInstance()->FileOpen(Path, ConvertFileToPlatformMode(OpenMode));
		FileMode = OpenMode;

		NEXUS_ASSERT(Handle, Default, "Failed to open file: %s", Path.C());
	}

	void File::Close()
	{
		if (!IsOpened())
		{
			return;
		}

		NEXUS_ASSERT(Exist && Handle, Default, "Failed to close file: %s", Path.C());

		Platform::GetInstance()->FileClose(Handle);
		FileMode = File::Mode::None;
		Handle = nullptr;

		NEXUS_ASSERT(Exist && !Handle, Default, "Failed to close file: %s", Path.C());
	}

	uint64 File::GetSize() const
	{
		NEXUS_ASSERT(Exist && Handle, Default, "Failed to query file size: %s", Path.C());

		return Platform::GetInstance()->FileSize(Handle);
	}

	void File::WriteByte(BufferView Data)
	{
		NEXUS_ASSERT(Exist && Handle && Data.GetPtr(), Default, "Failed to write file: %s", Path.C());

		Platform::GetInstance()->FileWriteByte(Handle, Data);
	}

	Buffer File::ReadByte() const
	{
		NEXUS_ASSERT(Exist && Handle, Default, "Failed to read file: %s", Path.C());

		return Platform::GetInstance()->FileReadByte(Handle);
	}

	void File::WriteText(StringView Text)
	{
		NEXUS_ASSERT(Exist && Handle && Text.C(), Default, "Failed to write file: %s", Path.C());

		Platform::GetInstance()->FileWriteText(Handle, Text);
	}

	String File::ReadText() const
	{
		NEXUS_ASSERT(Exist && Handle, Default, "Failed to read file: %s", Path.C());

		return Platform::GetInstance()->FileReadText(Handle);
	}

	void File::SetPath(StringView Value)
	{
		Path = Value;
		Path::Normalize(Path);
	}
}
