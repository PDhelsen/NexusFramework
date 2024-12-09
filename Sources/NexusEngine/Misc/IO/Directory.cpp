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

	bool Directory::Create()
	{
		if (Exist)
		{
			return true;
		}

		bool Result = Platform::GetInstance()->DirectoryCreate(Path);
		Refresh();

		NEXUS_ASSERT(Result && Exist, "Failed to create directory: %s", Path.C());

		return Result;
	}

	bool Directory::Move(StringView Target)
	{
		if (Path == Target)
		{
			return true;
		}

		NEXUS_ASSERT(Exist && !Path::Exist(Target), "Failed to move directory: from %s to %s", Path.C(), Target.C());

		bool Result = Platform::GetInstance()->DirectoryMove(Path, Target);
		Path = Target.ToString();
		Refresh();

		NEXUS_ASSERT(Result && Exist, "Failed to move directory: %s", Path.C());

		return Result;
	}

	bool Directory::Delete()
	{
		if (!Exist)
		{
			return true;
		}

		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.Delete();
			}

			if (Path::IsFile(It))
			{
				File F = File(It);
				F.Delete();
			}
		}

		bool Result = Platform::GetInstance()->DirectoryDelete(Path);
		Refresh();

		NEXUS_ASSERT(Result && !Exist, "Failed to delete directory: %s", Path.C());

		return Result;
	}

	List<String> Directory::GetContent(bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Directory doesn't exist: %s", Path.C());

		List<String> Result = List<String>(GetCount());
		GetContent(Result, Recursive);
		return Result;
	}

	void Directory::GetContent(List<String>& Result, bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Directory doesn't exist: %s", Path.C());

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
		NEXUS_ASSERT(Exist, "Directory doesn't exist: %s", Path.C());

		List<String> Result = List<String>(GetCount());
		GetFiles(Result, Recursive);
		return Result;
	}

	void Directory::GetFiles(List<String>& Result, bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Directory doesn't exist: %s", Path.C());

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
		NEXUS_ASSERT(Exist, "Directory doesn't exist: %s", Path.C());

		List<String> Result = List<String>(GetCount());
		GetDirectories(Result, Recursive);
		return Result;
	}

	void Directory::GetDirectories(List<String>& Result, bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Directory doesn't exist: %s", Path.C());

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
}
