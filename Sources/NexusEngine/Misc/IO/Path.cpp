#include "Core/NexusEnginePch.h"
#include "Path.h"

#include "Platform/Platform.h"

namespace NxEn
{
	Path::Path()
	{
	}

	Path::Path(StringView Path)
		: Data(Path.C(), Path.GetCount())
	{
	}

	Path::~Path()
	{
	}

	Path::operator StringView() const
	{
		return ToView();
	}

	bool Path::operator==(const Path& Other) const
	{
		return Data == Other.Data;
	}

	bool Path::operator!=(const Path& Other) const
	{
		return Data != Other.Data;
	}

	Path& Path::operator+=(const Path& Other)
	{
		Data += Other.Data;
		return *this;
	}

	Path& Path::operator+=(StringView Other)
	{
		Data += Other;
		return *this;
	}

	Path& Path::operator-=(const Path& Other)
	{
		Data -= Other.Data;
		return *this;
	}

	Path& Path::operator-=(StringView Other)
	{
		Data -= Other;
		return *this;
	}

	Path& Path::ChangeFileName(StringView File)
	{
		Path::ChangeFileName(Data, File);
		return *this;
	}

	Path& Path::ChangeDirectoryPath(StringView Directory)
	{
		Path::ChangeDirectoryPath(Data, Directory);
		return *this;
	}

	Path& Path::ChangeExtension(StringView Extension)
	{
		Path::ChangeExtension(Data, Extension);
		return *this;
	}

	Path& Path::ConvertAbsoluteToRelative(StringView Root)
	{
		Path::ConvertAbsoluteToRelative(Data, Root);
		return *this;
	}

	Path& Path::ConvertRelativeToAbsolute(StringView Root)
	{
		Path::ConvertRelativeToAbsolute(Data, Root);
		return *this;
	}

	Path& Path::Resolve()
	{
		Path::Resolve(Data);
		return *this;
	}

	Path& Path::Normalize(bool Directory)
	{
		Path::Normalize(Data, Directory);
		return *this;
	}

	bool Path::Exist() const
	{
		return Path::Exist(Data);
	}

	Path::Type Path::GetType() const
	{
		return Path::GetType(Data);
	}

	bool Path::IsFile() const
	{
		return Path::IsFile(Data);
	}

	bool Path::IsDirectory() const
	{
		return Path::IsDirectory(Data);
	}

	bool Path::IsAbsolute() const
	{
		return Path::IsAbsolute(Data);
	}

	bool Path::IsRelative() const
	{
		return Path::IsRelative(Data);
	}

	bool Path::HasExtension(StringView Extension) const
	{
		return Path::HasExtension(Data, Extension);
	}

	StringView Path::GetFileName(bool Extension) const
	{
		return Path::GetFileName(Data, Extension);
	}

	StringView Path::GetDirectoryPath() const
	{
		return Path::GetDirectoryPath(Data);
	}

	StringView Path::GetDirectoryName() const
	{
		return Path::GetDirectoryName(Data);
	}

	StringView Path::GetExtension() const
	{
		return Path::GetExtension(Data);
	}

	StringView Path::GetDrive() const
	{
		return Path::GetDrive(Data);
	}

	String Path::ChangeFileName(StringView Path, StringView File)
	{
		String Result = Path.ToString();
		Path::ChangeFileName(Result, File);
		return Result;
	}

	void Path::ChangeFileName(String& Path, StringView File)
	{
		StringView Substring = Path::GetFileName(Path, true);
		Path.Replace(Substring, File);
	}

	String Path::ChangeDirectoryPath(StringView Path, StringView Directory)
	{
		String Result = Path.ToString();
		Path::ChangeDirectoryPath(Result, Directory);
		return Result;
	}

	void Path::ChangeDirectoryPath(String& Path, StringView Directory)
	{
		StringView Substring = Path::GetDirectoryPath(Path);
		Path.Replace(Substring, Directory);
	}

	String Path::ChangeExtension(StringView Path, StringView Extension)
	{
		String Result = Path.ToString();
		Path::ChangeExtension(Result, Extension);
		return Result;
	}

	void Path::ChangeExtension(String& Path, StringView Extension)
	{
		StringView Substring = Path::GetExtension(Path);
		Path.Replace(Substring, Extension);
	}

	String Path::ConvertAbsoluteToRelative(StringView Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return String::Empty;
		}
		
		String Result = Path.ToString();
		Path::ConvertAbsoluteToRelative(Result, Root);
		return Result;
	}

	void Path::ConvertAbsoluteToRelative(String& Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return;
		}

		StringView Common = StringUtility::Common(Path, Root);
		StringView Remaining = Root.ToView(Common.GetCount(), Root.GetCount() - Common.GetCount());
		List<StringView> Directories = StringUtility::SplitAll(Remaining, "/");
		String ToReplaceWith = String(Directories.GetCount() * 3);
		for (auto It : Directories)
		{
			ToReplaceWith += "../";
		}

		Path.Replace(Common, ToReplaceWith);
	}

	String Path::ConvertRelativeToAbsolute(StringView Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return String::Empty;
		}

		String Result = Root + Path;
		Path::Resolve(Result);
		return Result;
	}

	void Path::ConvertRelativeToAbsolute(String& Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return;
		}

		Path = Root + Path;
		Path::Resolve(Path);
	}

	String Path::Resolve(StringView Path)
	{
		String Result = Path.ToString();
		Path::Resolve(Result);
		return Result;
	}

	void Path::Resolve(String& Path)
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
			StringView First = Directories[Index - Count];
			StringView Last = Directories[Index + Count - 1];
			StringView ToRemove = StringView(First.C(), (Last.C() + Last.GetCount() - First.C() + 1));
			Path.Remove(ToRemove);
		}
	}

	String Path::Normalize(StringView Path, bool Directory)
	{
		String Result = Path.ToString();
		Path::Normalize(Result, Directory);
		return Result;
	}

	void Path::Normalize(String& Path, bool Directory)
	{
		if (Directory && !Path.End("/"))
		{
			Path += "/";
		}
		Path.Replace("\\", "/");
		Path.Replace("//", "/");
	}

	bool Path::Exist(StringView Path)
	{
		return Platform::GetInstance()->GetPathType(Path) != Platform::PathType::None;
	}

	Path::Type Path::GetType(StringView Path)
	{
		if (StringUtility::End(Path, "/"))
		{
			return Type::Directory;
		}

		return Type::File;
	}

	bool Path::IsFile(StringView Path)
	{
		return Path::GetType(Path) == Type::File;
	}

	bool Path::IsDirectory(StringView Path)
	{
		return Path::GetType(Path) == Type::Directory;
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

	Path Path::GetWorkingDirectory()
	{
		return Path(Platform::GetInstance()->GetWorkingDirectory()).Normalize(true);
	}

	bool operator==(const Path& A, StringView B)
	{
		return A.ToView() == B;
	}

	bool operator==(StringView A, const Path& B)
	{
		return A == B.ToView();
	}

	bool operator!=(const Path& A, StringView B)
	{
		return A.ToView() == B;
	}

	bool operator!=(StringView A, const Path& B)
	{
		return A == B.ToView();
	}

	Path operator+(const Path& A, const Path& B)
	{
		return Path(A.ToView() + B.ToView());
	}

	Path operator+(const Path& A, StringView B)
	{
		return Path(A.ToView() + B);
	}

	Path operator+(StringView A, const Path& B)
	{
		return Path(A + B.ToView());
	}

	Path operator-(const Path& A, const Path& B)
	{
		return Path(A.ToView() - B.ToView());
	}

	Path operator-(const Path& A, StringView B)
	{
		return Path(A.ToView() - B);
	}

	Path operator-(StringView A, const Path& B)
	{
		return Path(A - B.ToView());
	}
}
