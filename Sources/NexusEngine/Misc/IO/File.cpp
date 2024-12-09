#include "Core/NexusEnginePch.h"
#include "File.h"

#include "Platform/Platform.h"

namespace NxEn
{
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

	void File::Refresh()
	{
		Exist = Path::Exist(Path);
	}

	bool File::Create()
	{
		if (Exist)
		{
			return Open();
		}

		NEXUS_ASSERT(!Handle, "Failed to create file: %s", Path.C());

		Handle = Platform::GetInstance()->FileCreate(Path);
		Refresh();

		NEXUS_ASSERT(Handle && Exist, "Failed to create file: %s", Path.C());

		return Handle != nullptr;
	}

	bool File::Move(StringView Target)
	{
		if (Path == Target)
		{
			return true;
		}

		NEXUS_ASSERT(!Handle && Exist && !Path::Exist(Target), "Failed to move file: from %s to %s", Path.C(), Target.C());

		bool Result = Platform::GetInstance()->FileMove(Path, Target);
		Path = Target.ToString();
		Refresh();

		NEXUS_ASSERT(Result && Exist, "Failed to move file: %s", Path.C());

		return Result;
	}

	bool File::Delete()
	{
		if (!Exist)
		{
			return true;
		}

		NEXUS_ASSERT(!Handle, "Failed to delete file: %s", Path.C());

		bool Result = Platform::GetInstance()->FileDelete(Path);
		Refresh();

		NEXUS_ASSERT(Result && !Exist, "Failed to delete file: %s", Path.C());

		return Result;
	}

	bool File::Open()
	{
		if (!Exist)
		{
			return Create();
		}

		if (Handle)
		{
			return true;
		}

		Handle = Platform::GetInstance()->FileOpen(Path);

		NEXUS_ASSERT(Handle, "Failed to open file: %s", Path.C());

		return Handle != nullptr;
	}

	bool File::Close()
	{
		if (!Handle)
		{
			return true;
		}

		bool Result = Platform::GetInstance()->FileClose(Handle);
		Handle = nullptr;

		NEXUS_ASSERT(Result, "Failed to close file: %s", Path.C());

		return Result;
	}
}
