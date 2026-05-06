#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/Directory.h"

#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	Directory::Directory(StringView Path)
		: Path()
	{
		SetPath(Path);
	}

	Directory::operator bool() const
	{
		return Exists();
	}

	bool Directory::operator==(const Directory& Other) const
	{
		return Path == Other.Path;
	}

	bool Directory::operator!=(const Directory& Other) const
	{
		return !(*this == Other);
	}

	bool Directory::Exists() const
	{
		return Path::Exist(Path);
	}

	void Directory::EnsureParent() const
	{
		Path::EnsureParent(Path);
	}

	void Directory::Create()
	{
		if (Path.IsEmpty())
		{
			NEXUS_LOG(Error, Default, "Impossible to create directory with empty path");
			return;
		}

		if (Exists())
		{
			return;
		}

		NEXUS_ASSERT(!Exists(), Default, "Failed to create directory: %s", Path.C());

		Path::EnsureParent(Path);
		Platform::GetInstance()->DirectoryCreate(Path);

		NEXUS_ASSERT(Exists(), Default, "Failed to create directory: %s", Path.C());
	}

	void Directory::Move(StringView Target, bool Override)
	{
		if (Path.IsEmpty() || Target.IsEmpty() || Path == Target)
		{
			NEXUS_LOG(Error, Default, "Impossible to move directory with empty path");
			return;
		}

		NEXUS_ASSERT(Exists() && Path != Target && !Path::Exist(Target), Default, "Failed to move directory: %s", Path.C());

		Path::EnsureParent(Target);
		Platform::GetInstance()->DirectoryMove(Path, Target, Override);
		SetPath(Target);

		NEXUS_ASSERT(Exists(), Default, "Failed to move directory: %s", Path.C());
	}

	void Directory::Copy(StringView Target, bool Override)
	{
		if (Path.IsEmpty() || Target.IsEmpty() || Path == Target)
		{
			NEXUS_LOG(Error, Default, "Impossible to copy directory with empty path");
			return;
		}

		NEXUS_ASSERT(Exists() && Path != Target && !Path::Exist(Target), Default, "Failed to copy directory: %s", Path.C());

		Path::EnsureParent(Target);
		Platform::GetInstance()->DirectoryCreate(Target);

		List<String> Content = List<String>();
		GetContent(Content);
		for (auto& It : Content)
		{
			if (Path::IsDirectory(It))
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.Copy(StringUtility::Replace(It, Path, Target), Override);
			}

			if (Path::IsFile(It))
			{
				File FileInDirectory = File(It);
				FileInDirectory.Copy(StringUtility::Replace(It, Path, Target), Override);
			}
		}

		NEXUS_ASSERT(Exists(), Default, "Failed to copy directory: %s", Path.C());
	}

	void Directory::Delete()
	{
		if (Path.IsEmpty())
		{
			NEXUS_LOG(Error, Default, "Impossible to delete directory with empty path");
			return;
		}

		if (!Exists())
		{
			return;
		}

		NEXUS_ASSERT(Exists(), Default, "Failed to delete directory: %s", Path.C());

		List<String> Content = List<String>();
		GetContent(Content);
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

		Platform::GetInstance()->DirectoryDelete(Path);

		NEXUS_ASSERT(!Exists(), Default, "Failed to delete directory: %s", Path.C());
	}

	List<String> Directory::GetContent(bool Recursive) const
	{
		NEXUS_ASSERT(Exists(), Default, "Failed to get content of directory: %s", Path.C());

		List<String> Content = List<String>();
		GetContent(Content, Recursive);
		return Content;
	}

	void Directory::GetContent(List<String>& Content, bool Recursive) const
	{
		NEXUS_ASSERT(Exists(), Default, "Failed to get content of directory: %s", Path.C());

		GetContent(Content, Recursive, true, true);
	}

	List<String> Directory::GetFiles(bool Recursive) const
	{
		NEXUS_ASSERT(Exists(), Default, "Failed to get content of directory: %s", Path.C());

		List<String> Content = List<String>();
		GetFiles(Content, Recursive);
		return Content;
	}

	void Directory::GetFiles(List<String>& Content, bool Recursive) const
	{
		NEXUS_ASSERT(Exists(), Default, "Failed to get content of directory: %s", Path.C());

		GetContent(Content, Recursive, true, false);
	}

	List<String> Directory::GetDirectories(bool Recursive) const
	{
		NEXUS_ASSERT(Exists(), Default, "Failed to get content of directory: %s", Path.C());

		List<String> Content = List<String>();
		GetDirectories(Content, Recursive);
		return Content;
	}

	void Directory::GetDirectories(List<String>& Content, bool Recursive) const
	{
		NEXUS_ASSERT(Exists(), Default, "Failed to get content of directory: %s", Path.C());

		GetContent(Content, Recursive, false, true);
	}

	void Directory::SetPath(StringView Value)
	{
		Path = Value;
		Path += Path::SeparatorFolder;
		Path::Normalize(Path);
	}

	void Directory::GetContent(List<String>& Result, bool Recursive, bool GetFile, bool GetDirectory) const
	{
		List<String> Content = Platform::GetInstance()->DirectoryContent(Path);
		for (auto& It : Content)
		{
			bool IsFile = Path::IsFile(It);
			bool IsDirectory = Path::IsDirectory(It);

			if ((IsFile && GetFile) || (IsDirectory && GetDirectory))
			{
				Result.Append(It);
			}

			if (Recursive && IsDirectory)
			{
				Directory SubDirectory = Directory(It);
				SubDirectory.GetFiles(Result, Recursive);
			}
		}
	}
}
