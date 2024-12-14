#include "Core/NexusEnginePch.h"
#include "File.h"

#include "Platform/Platform.h"

namespace NxEn
{
	Platform::FileMode ConvertFileToPlatformMode(File::Mode Mode)
	{
		switch (Mode)
		{
		case NxEn::File::Mode::Read: return Platform::FileMode::Read;
		case NxEn::File::Mode::Write: return Platform::FileMode::Write;
		case NxEn::File::Mode::Append: return Platform::FileMode::Append;
		}

		return Platform::FileMode::Read;
	}

	File::File(StringView Path)
		: Path(Path.ToString()), Exist(false), Handle(nullptr)
	{
		Refresh();
	}

	File::File(String&& Path)
		: Path(Move(Path)), Exist(false), Handle(nullptr)
	{
		Refresh();
	}

	File::~File()
	{
		NEXUS_ASSERT(!Handle, "Delete file while it is still open: %s", Path.C());
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

	bool File::Create(bool KeepOpen)
	{
		if (Exist)
		{
			if (KeepOpen)
			{
				return Open(Mode::Write);
			}
			else
			{
				return true;
			}
		}

		NEXUS_ASSERT(!Exist && !Handle, "Failed to create file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileCreate(Path, KeepOpen ? &Handle : nullptr);
		Refresh();

		NEXUS_ASSERT(Result && Exist && !Handle, "Failed to create file: %s", Path.C());

		return Result;
	}

	bool File::Move(StringView Target, bool Override, bool CloseIfOpen)
	{
		if (Path == Target)
		{
			return true;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NEXUS_ASSERT(Exist && !Handle && Path != Target && !Path::Exist(Target), "Failed to move file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileMove(Path, Target, Override);
		Path = Target.ToString();
		Refresh();

		NEXUS_ASSERT(Result && Exist && !Handle, "Failed to move file: %s", Path.C());

		return Result;
	}

	bool File::Copy(StringView Target, bool Override, bool CloseIfOpen)
	{
		if (Path == Target)
		{
			return true;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NEXUS_ASSERT(Exist && !Handle && Path != Target && !Path::Exist(Target), "Failed to copy file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileCopy(Path, Target, Override);
		Refresh();

		NEXUS_ASSERT(Result && Exist && !Handle, "Failed to copy file: %s", Path.C());

		return Result;
	}

	bool File::Delete(bool CloseIfOpen)
	{
		if (!Exist)
		{
			return true;
		}

		if (CloseIfOpen && Handle)
		{
			Close();
		}

		NEXUS_ASSERT(Exist && !Handle, "Failed to delete file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileDelete(Path);
		Refresh();

		NEXUS_ASSERT(Result && !Exist && !Handle, "Failed to delete file: %s", Path.C());

		return Result;
	}

	bool File::Open(Mode OpenMode, bool CreateIfDontExist)
	{
		if (Handle)
		{
			return true;
		}

		if (CreateIfDontExist && !Exist)
		{
			return Create(true);
		}

		NEXUS_ASSERT(Exist && !Handle, "Failed to open file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileOpen(Path, &Handle, ConvertFileToPlatformMode(OpenMode));

		NEXUS_ASSERT(Exist && Handle, "Failed to open file: %s", Path.C());

		return Result;
	}

	bool File::Close()
	{
		if (!Handle)
		{
			return true;
		}

		NEXUS_ASSERT(Exist && Handle, "Failed to close file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileClose(Handle);
		Handle = nullptr;

		NEXUS_ASSERT(Result && Exist && !Handle, "Failed to close file: %s", Path.C());

		return Result;
	}

	uint64 File::GetSize()
	{
		NEXUS_ASSERT(Exist && Handle, "Failed to query file size: %s", Path.C());

		uint64 Size = 0;
		bool Result = Platform::GetInstance()->FileSize(Handle, &Size);

		NEXUS_ASSERT(Result, "Failed to query file size: %s", Path.C());

		return Size;
	}

	void File::WriteByte(BufferView<Byte> Data)
	{
		NEXUS_ASSERT(Exist && Handle && Data.GetPtr(), "Failed to write file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileWriteByte(Handle, Data);

		NEXUS_ASSERT(Result, "Failed to write file: %s", Path.C());
	}

	Buffer<Byte> File::ReadByte(Allocator* Allctr)
	{
		NEXUS_ASSERT(Exist && Handle, "Failed to read file: %s", Path.C());

		Buffer<Byte> Data = Buffer<Byte>(GetSize(), Allctr);

		bool Result = Platform::GetInstance()->FileReadByte(Handle, Data);

		NEXUS_ASSERT(Result, "Failed to read file: %s", Path.C());

		return Data;
	}
}
