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

	Directory::~Directory()
	{
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

	Directory& Directory::Refresh()
	{
		Exist = Path::Exist(Path);
		if (Exist)
		{
			Platform::GetInstance()->DirectoryContent(Path, Content);
		}

		return *this;
	}

	bool Directory::Create()
	{
		if (Exist)
		{
			return true;
		}

		NEXUS_ASSERT(!Exist, "Failed to create directory: %s", Path.C());

		bool Result = Platform::GetInstance()->DirectoryCreate(Path);
		Refresh();

		NEXUS_ASSERT(Result && Exist, "Failed to create directory: %s", Path.C());

		return Result;
	}

	bool Directory::Move(StringView Target, bool Override)
	{
		if (Path == Target)
		{
			return true;
		}

		NEXUS_ASSERT(Exist && Path != Target && !Path::Exist(Target), "Failed to move directory: %s", Path.C());

		bool Result = Platform::GetInstance()->DirectoryMove(Path, Target, Override);
		Path = Target.ToString();
		Refresh();

		NEXUS_ASSERT(Result && Exist, "Failed to move directory: %s", Path.C());

		return Result;
	}

	bool Directory::Copy(StringView Target, bool Override)
	{
		if (Path == Target)
		{
			return true;
		}

		NEXUS_ASSERT(Exist && Path != Target && !Path::Exist(Target), "Failed to copy directory: %s", Path.C());

		bool Result = Platform::GetInstance()->DirectoryCreate(Target);

		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.Copy(String(It).Replace(Path, Target), Override);
			}

			if (Path::IsFile(It))
			{
				File FileInDirectory = File(It);
				FileInDirectory.Copy(String(It).Replace(Path, Target), Override);
			}
		}

		Refresh();

		NEXUS_ASSERT(Result && Exist, "Failed to copy directory: %s", Path.C());

		return Result;
	}

	bool Directory::Delete()
	{
		if (!Exist)
		{
			return true;
		}

		NEXUS_ASSERT(Exist, "Failed to delete directory: %s", Path.C());

		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.Delete();
			}

			if (Path::IsFile(It))
			{
				File FileInDirectory = File(It);
				FileInDirectory.Delete();
			}
		}

		bool Result = Platform::GetInstance()->DirectoryDelete(Path);
		Refresh();

		NEXUS_ASSERT(Result && !Exist, "Failed to delete directory: %s", Path.C());

		return Result;
	}

	List<String> Directory::GetContent(bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Failed to get content of directory: %s", Path.C());

		List<String> Result = List<String>(GetCount());
		GetContent(Result, Recursive);
		return Result;
	}

	void Directory::GetContent(List<String>& Result, bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Failed to get content of directory: %s", Path.C());

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
		NEXUS_ASSERT(Exist, "Failed to get content of directory: %s", Path.C());

		List<String> Result = List<String>(GetCount());
		GetFiles(Result, Recursive);
		return Result;
	}

	void Directory::GetFiles(List<String>& Result, bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Failed to get content of directory: %s", Path.C());

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
		NEXUS_ASSERT(Exist, "Failed to get content of directory: %s", Path.C());

		List<String> Result = List<String>(GetCount());
		GetDirectories(Result, Recursive);
		return Result;
	}

	void Directory::GetDirectories(List<String>& Result, bool Recursive) const
	{
		NEXUS_ASSERT(Exist, "Failed to get content of directory: %s", Path.C());

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
