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
		List<String> Contents = List<String>(GetCount());
		for (auto& It : Content)
		{
			Contents.Append(It);

			if (Recursive && Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				Contents.AppendRange(SubDirectory.GetContent(Recursive));
			}
		}
		return Contents;
	}

	List<String> Directory::GetFiles(bool Recursive) const
	{
		List<String> Files = List<String>(GetCount());
		for (auto& It : Content)
		{
			if (Path::IsFile(It))
			{
				Files.Append(It);
			}

			if (Recursive && Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				Files.AppendRange(SubDirectory.GetFiles(Recursive));
			}
		}
		return Files;
	}

	List<String> Directory::GetDirectories(bool Recursive) const
	{
		List<String> Directories = List<String>(GetCount());
		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Directories.Append(It);
			}

			if (Recursive && Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				Directories.AppendRange(SubDirectory.GetDirectories(Recursive));
			}
		}
		return Directories;
	}

	bool Directory::Create()
	{
		Refresh();
		if (Exist)
		{
			return true;
		}

		bool Result = Platform::GetInstance()->DirectoryCreate(Path);

		if (Result)
		{
			Refresh();
		}

		return Result;
	}

	bool Directory::Move(StringView Target)
	{
		Refresh();
		if (!Exist)
		{
			return false;
		}

		bool Result = Platform::GetInstance()->DirectoryMove(Path, Target);

		Path = Target.ToString();
		if (Result)
		{
			Refresh();
		}

		return Result;
	}

	bool Directory::Delete()
	{
		Refresh();
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

		if (Result)
		{
			Refresh();
		}

		return Result;
	}
}
