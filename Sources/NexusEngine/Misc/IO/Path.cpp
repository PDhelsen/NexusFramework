#include "Core/NexusEnginePch.h"
#include "Path.h"

#include "Platform/Platform.h"

namespace NxEn
{
	bool Path::Exist(StringView Path)
	{
		return Platform::GetInstance()->GetPathInfo(Path) != PlatformPathInfo::None;
	}

	bool Path::IsFile(StringView Path)
	{
		return !Path::IsDirectory(Path);
	}

	bool Path::IsDirectory(StringView Path)
	{
		return StringUtility::End(Path, "/");
	}

	bool Path::IsAbsolute(StringView Path)
	{
		return StringUtility::Contains(Path, ":/");
	}

	bool Path::IsRelative(StringView Path)
	{
		return !Path::IsAbsolute(Path);
	}

	bool Path::HasExtension(StringView Path, StringView Extension)
	{
		return StringUtility::End(Path, Extension);
	}

	StringView Path::GetFileName(StringView Path, bool Extension)
	{
		if (!Path::IsFile(Path))
		{
			return StringView();
		}

		List<StringView> Split = StringUtility::SplitAll(Path, "/");
		StringView Last = Split.Last();
		return Extension ? Last : Last.Split(".");
	}

	StringView Path::GetDirectoryPath(StringView Path)
	{
		if (Path::IsDirectory(Path))
		{
			return Path;
		}

		List<StringView> Split = StringUtility::SplitAll(Path, "/");
		StringView Last = Split.Last();
		return Path.ToView(0, Path.GetCount() - Last.GetCount());
	}

	StringView Path::GetDirectoryName(StringView Path)
	{
		List<StringView> Split = StringUtility::SplitAll(Path, "/");
		return IsDirectory(Path) ? Split.Last() : Split.Get(Split.GetCount() - 2);
	}

	StringView Path::GetExtension(StringView Path)
	{
		if (!Path::IsFile(Path))
		{
			return StringView();
		}

		return StringUtility::Split(Path, ".", 1);
	}

	StringView Path::GetDrive(StringView Path)
	{
		if (!Path::IsAbsolute(Path))
		{
			String::Empty;
		}

		return StringView(Path.C(), 1);
	}

	String Path::Combine(StringView First, StringView Second, bool Directory)
	{
		String Result = String(First.GetCount() + 1 + Second.GetCount() + 1);
		Result += First;
		if (!First.End("/"))
		{
			Result += "/";
		}
		Result += Second;
		if (Directory)
		{
			Result += "/";
		}
		return Result;
	}

	String Path::ChangeFileName(StringView Path, StringView FileName)
	{
		StringView Substring = Path::GetFileName(Path, true);

		String Result = String(Path.C(), Path.GetCount());
		Result.Replace(Substring, FileName);
		return Result;
	}

	String Path::ChangeDirectoryPath(StringView Path, StringView DirectoryPath)
	{
		StringView Substring = Path::GetDirectoryPath(Path);

		String Result = String(Path.C(), Path.GetCount());
		Result.Replace(Substring, DirectoryPath);
		return Result;
	}

	String Path::ChangeExtension(StringView Path, StringView Extension)
	{
		StringView Substring = Path::GetExtension(Path);

		String Result = String(Path.C(), Path.GetCount());
		Result.Replace(Substring, Extension);
		return Result;
	}

	String Path::ConvertAbsoluteToRelative(StringView Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return String::Empty;
		}

		StringView Common = StringUtility::Common(Path, Root);
		StringView Remaining = Root.ToView(Common.GetCount(), Root.GetCount() - Common.GetCount());
		List<StringView> Directories = StringUtility::SplitAll(Remaining, "/");
		String ToReplaceWith = String(Directories.GetCount() * 3);
		for (auto It : Directories)
		{
			ToReplaceWith += "../";
		}

		String Result = Path.ToString();
		Result.Replace(Common, ToReplaceWith);
		return Result;
	}

	String Path::ConvertRelativeToAbsolute(StringView Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return String::Empty;
		}

		List<StringView> Up = StringUtility::SplitAll(Path, "/");
		List<StringView> Down = StringUtility::SplitAll(Root, "/");

		uint64 ToGoUp = 0;
		for (auto& It : Up)
		{
			if (It != "..")
			{
				break;
			}

			ToGoUp++;
		}

		Up.RemoveRange(0, ToGoUp);
		Down.RemoveRange(Down.GetCount() - ToGoUp, ToGoUp);

		Down.AppendRange(Up);
		return Path::Combine<StringView>(Down, true);
	}

	String Path::Resolve(StringView Path)
	{
		List<StringView> Directories = StringUtility::SplitAll(Path, "/");
		int64 Index = -1;
		uint64 Count = 0;

		uint64 Iter = 0;
		for (auto& It : Directories)
		{
			if (It == "..")
			{
				if (Index == -1)
				{
					Index = Iter;
				}
				Count++;
			}
			Iter++;
		}

		if (Index != -1)
		{
			Index -= Count;
			Count *= 2;
			Directories.RemoveRange(Index, Count);
		}

		return Path::Combine<StringView>(Directories, true);
	}

	String Path::Normalize(StringView Path, bool Directory)
	{
		String Result = Path.ToString();
		if (Directory && !Result.End("/"))
		{
			Result += "/";
		}
		Result.Replace("\\", "/");
		Result.Replace("//", "/");
		return Result;
	}

	String Path::GetWorkingDirectory()
	{
		return Path::Normalize(Platform::GetInstance()->GetWorkingDirectory(), true);
	}
}
