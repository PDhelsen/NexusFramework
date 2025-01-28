#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/File.h"

#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/IO/Path.h"

namespace NxFr
{
	Platform::FileMode ConvertFileToPlatformMode(File::Mode Mode)
	{
		switch (Mode)
		{
		case NxFr::File::Mode::Read: return Platform::FileMode::Read;
		case NxFr::File::Mode::Write: return Platform::FileMode::Write;
		case NxFr::File::Mode::Append: return Platform::FileMode::Append;
		}

		return Platform::FileMode::Read;
	}

	File::File(StringView Path)
		: Path(Path.ToString()), Exist(false), Handle(nullptr)
	{
		Refresh();
	}

	File::~File()
	{
		NEXUS_ASSERT(!Handle, Default, "Delete file while it is still open: %s", Path.C());
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

		Handle = Platform::GetInstance()->FileCreate(Path, KeepOpen);
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

		Platform::GetInstance()->FileMove(Path, Target, Override);
		Path = Target.ToString();
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

		Platform::GetInstance()->FileCopy(Path, Target, Override);
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

		NEXUS_ASSERT(Handle, Default, "Failed to open file: %s", Path.C());
	}

	void File::Close()
	{
		if (!Handle)
		{
			return;
		}

		NEXUS_ASSERT(Exist && Handle, Default, "Failed to close file: %s", Path.C());

		Platform::GetInstance()->FileClose(Handle);
		Handle = nullptr;

		NEXUS_ASSERT(Exist && !Handle, Default, "Failed to close file: %s", Path.C());
	}

	uint64 File::GetSize()
	{
		NEXUS_ASSERT(Exist && Handle, Default, "Failed to query file size: %s", Path.C());

		uint64 Size = Platform::GetInstance()->FileSize(Handle);

		return Size;
	}

	void File::WriteByte(BufferView<Byte> Data)
	{
		NEXUS_ASSERT(Exist && Handle && Data.GetPtr(), Default, "Failed to write file: %s", Path.C());

		Platform::GetInstance()->FileWriteByte(Handle, Data);
	}

	Buffer<Byte> File::ReadByte()
	{
		NEXUS_ASSERT(Exist && Handle, Default, "Failed to read file: %s", Path.C());

		Buffer<Byte> Data = Platform::GetInstance()->FileReadByte(Handle);

		return Data;
	}

	void File::WriteText(StringView Text)
	{
		NEXUS_ASSERT(Exist && Handle && Text.C(), Default, "Failed to write file: %s", Path.C());

		Platform::GetInstance()->FileWriteText(Handle, Text);
	}

	String File::ReadText()
	{
		NEXUS_ASSERT(Exist && Handle, Default, "Failed to read file: %s", Path.C());

		String Text = Platform::GetInstance()->FileReadText(Handle);

		return Text;
	}
}
