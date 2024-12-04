#include "Core/NexusEnginePch.h"
#include "File.h"

#include "Platform/Platform.h"

namespace NxEn
{
	File::File(StringView Path)
		: Path(Path.ToString()), Exist(false)
	{
		Refresh();
	}

	File::File(String&& Path)
		: Path(Move(Path)), Exist(false)
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
			return true;
		}

		bool Result = Platform::GetInstance()->FileCreate(Path);
		Refresh();
		return Result;
	}

	bool File::Move(StringView Target)
	{
		if (!Exist)
		{
			return false;
		}

		bool Result = Platform::GetInstance()->FileMove(Path, Target);
		Path = Target.ToString();
		Refresh();
		return Result;
	}

	bool File::Delete()
	{
		if (!Exist)
		{
			return false;
		}

		bool Result = Platform::GetInstance()->FileDelete(Path);
		Refresh();
		return Result;
	}
}
