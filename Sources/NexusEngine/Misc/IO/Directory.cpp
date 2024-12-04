#include "Core/NexusEnginePch.h"
#include "Directory.h"

#include "Platform/Platform.h"

namespace NxEn
{
	Directory::Directory(StringView Path)
		: Path(Path.ToString()), Exist(false), Content()
	{
		Refresh();
	}

	Directory::Directory(String&& Path)
		: Path(Move(Path)), Exist(false), Content()
	{
		Refresh();
	}

	Directory::operator bool() const
	{
		return Exist;
	}

	bool Directory::operator==(const Directory& Other) const
	{
		return Path == Other.Path;
	}

	bool Directory::operator!=(const Directory& Other) const
	{
		return !(*this == Other);
	}

	void Directory::Refresh()
	{
		Exist = Path::Exist(Path);
		if (Exist)
		{
			Content = Platform::GetInstance()->DirectoryContent(Path);
		}
	}

	List<String> Directory::GetContent(bool Recursive) const
	{
		List<String> Result = List<String>(GetCount());
		GetContent(Result, Recursive);
		return Result;
	}

	void Directory::GetContent(List<String>& Result, bool Recursive) const
	{
		for (auto& It : Content)
		{
			Result.Append(It);

			if (Recursive && Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.GetContent(Result, Recursive);
			}
		}
	}

	List<String> Directory::GetFiles(bool Recursive) const
	{
		List<String> Result = List<String>(GetCount());
		GetFiles(Result, Recursive);
		return Result;
	}

	void Directory::GetFiles(List<String>& Result, bool Recursive) const
	{
		for (auto& It : Content)
		{
			if (Path::IsFile(It))
			{
				Result.Append(It);
			}

			if (Recursive && Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.GetFiles(Result, Recursive);
			}
		}
	}

	List<String> Directory::GetDirectories(bool Recursive) const
	{
		List<String> Result = List<String>(GetCount());
		GetDirectories(Result, Recursive);
		return Result;
	}

	void Directory::GetDirectories(List<String>& Result, bool Recursive) const
	{
		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Result.Append(It);
			}

			if (Recursive && Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.GetDirectories(Result, Recursive);
			}
		}
	}

	bool Directory::Create()
	{
		if (Exist)
		{
			return true;
		}

		bool Result = Platform::GetInstance()->DirectoryCreate(Path);
		Refresh();
		return Result;
	}

	bool Directory::Move(StringView Target)
	{
		if (!Exist)
		{
			return false;
		}

		bool Result = Platform::GetInstance()->DirectoryMove(Path, Target);
		Path = Target.ToString();
		Refresh();
		return Result;
	}

	bool Directory::Delete()
	{
		if (!Exist)
		{
			return false;
		}

		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.Delete();
			}
		}

		bool Result = Platform::GetInstance()->DirectoryDelete(Path);
		Refresh();
		return Result;
	}
}
