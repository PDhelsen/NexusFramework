#include "Core/NexusFrameworkPch.h"
#include "Path.h"

#include "Platform/Platform.h"

namespace NxFr
{
	Path Path::GetWorkingDirectory()
	{
		return Path::ConvertStringToPath(Platform::GetInstance()->GetWorkingDirectory());
	}

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Static
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	String Path::Combine(StringView Base, StringView Element)
	{
		String Result = Base.ToString();
		Path::Combine(Result, Element);
		return Result;
	}

	void Path::Combine(String& Base, StringView Element)
	{
		if (!Base.End(SeparatorDirectory))
		{
			Base += SeparatorDirectory;
		}
		Base += Element;
		if (!Element.End(SeparatorDirectory) && !IsFile(Element))
		{
			Base += SeparatorDirectory;
		}
	}

	String Path::Previous(StringView Path, uint64 Count)
	{
		String Result = Path.ToString();
		Path::Previous(Result, Count);
		return Result;
	}

	void Path::Previous(String& Path, uint64 Count)
	{
		if (Count == 0)
		{
			return;
		}

		List<StringView> Split = StringUtility::SplitAll(Path, SeparatorDirectory);
		StringView Last = Split.Get(Split.GetCount() - 1 - Count);
		Path.Terminate(Last.C() - Path.C() + Last.GetCount() + 1);
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
			return StringUtility::Empty;
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
		List<StringView> Directories = StringUtility::SplitAll(Remaining, SeparatorDirectory);
		String ToReplaceWith = String(Directories.GetCount() * 3);
		for (auto It : Directories)
		{
			ToReplaceWith += SeparatorPrevious;
			ToReplaceWith += SeparatorDirectory;
		}

		Path.Replace(Common, ToReplaceWith);
	}

	String Path::ConvertRelativeToAbsolute(StringView Path, StringView Root)
	{
		if (!Path::IsDirectory(Root))
		{
			return StringUtility::Empty;
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
		List<StringView> Directories = StringUtility::SplitAll(Path, SeparatorDirectory);
		int64 Index = -1;
		uint64 Count = 0;

		uint64 Iter = 0;
		for (auto& It : Directories)
		{
			if (It == SeparatorPrevious)
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

	String Path::Normalize(StringView Path)
	{
		String Result = Path.ToString();
		Path::Normalize(Result);
		return Result;
	}

	void Path::Normalize(String& Path)
	{
		if (!Path::IsFile(Path) && !Path.End(SeparatorDirectory))
		{
			Path += SeparatorDirectory;
		}
		Path.Replace("\\", SeparatorDirectory);
		Path.Replace("//", SeparatorDirectory);
	}

	Path::Type Path::GetType(StringView Path)
	{
		if (StringUtility::End(Path, SeparatorDirectory))
		{
			return Type::Directory;
		}

		if (StringUtility::Contains(Path, SeparatorExtension) && !StringUtility::Contains(Path, SeparatorPrevious))
		{
			return Type::File;
		}

		return Type::Invalid;
	}

	bool Path::Exist(StringView Path)
	{
		return Platform::GetInstance()->GetPathType(Path) != Platform::PathType::None;
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
		return StringUtility::Contains(Path, SeparatorDrive);
	}

	bool Path::IsRelative(StringView Path)
	{
		return !Path::IsAbsolute(Path);
	}

	bool Path::HasExtension(StringView Path, StringView Extension)
	{
		return StringUtility::End(Path, Extension);
	}

	StringView Path::GetDrive(StringView Path)
	{
		return StringUtility::Split(Path, SeparatorDrive);
	}

	StringView Path::GetDirectoryPath(StringView Path)
	{
		if (Path::IsDirectory(Path))
		{
			return Path;
		}

		List<StringView> Split = StringUtility::SplitAll(Path, SeparatorDirectory);
		return Path.ToView(0, Path.GetCount() - Split.Last().GetCount());
	}

	StringView Path::GetDirectoryName(StringView Path)
	{
		List<StringView> Split = StringUtility::SplitAll(Path, SeparatorDirectory);
		return IsDirectory(Path) ? Split.Last() : Split.Get(Split.GetCount() - 2);
	}

	StringView Path::GetParent(StringView Path)
	{
		List<StringView> Split = StringUtility::SplitAll(Path, SeparatorDirectory);
		return Path.ToView(0, Path.GetCount() - Split.Last().GetCount() - Path::IsDirectory(Path));
	}

	StringView Path::GetFileName(StringView Path, bool Extension)
	{
		if (!Path::IsFile(Path))
		{
			return StringView();
		}

		List<StringView> Split = StringUtility::SplitAll(Path, SeparatorDirectory);
		return Extension ? Split.Last() : Split.Last().Split(SeparatorExtension);
	}

	StringView Path::GetExtension(StringView Path)
	{
		if (!Path::IsFile(Path))
		{
			return StringView();
		}

		return StringUtility::Split(Path, SeparatorExtension, 1);
	}

	List<StringView> Path::Split(StringView Path)
	{
		return StringUtility::SplitAll(Path, SeparatorDirectory);
	}

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Instance
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	Path::Path(StringView Path)
		: Data(Path.C(), Path.GetCount())
	{
	}

	Path::~Path()
	{
	}

	Path Path::ConvertStringToPath(String&& Temp)
	{
		Path Result("");
		Result.Data = Move(Temp);
		return Result;
	}

	Path::operator bool() const
	{
		return IsValid();
	}

	Path::operator StringView() const
	{
		return ToView();
	}

	Path& Path::operator+=(const Path& Other)
	{
		Join(Other);
		return *this;
	}

	Path& Path::operator+=(StringView Other)
	{
		Join(Other);
		return *this;
	}

	Path& Path::operator-=(uint64 Count)
	{
		Previous(Count);
		return *this;
	}

	Path& Path::Join(StringView Element)
	{
		Path::Combine(Data, Element);
		return *this;
	}

	Path& Path::Previous(uint64 Count)
	{
		Path::Previous(Data, Count);
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

	Path& Path::Normalize()
	{
		Path::Normalize(Data);
		return *this;
	}

	Path::Type Path::GetType() const
	{
		return Path::GetType(Data);
	}

	bool Path::Exist() const
	{
		return Path::Exist(Data);
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

	StringView Path::GetDrive() const
	{
		return Path::GetDrive(Data);
	}

	StringView Path::GetDirectoryPath() const
	{
		return Path::GetDirectoryPath(Data);
	}

	StringView Path::GetDirectoryName() const
	{
		return Path::GetDirectoryName(Data);
	}

	StringView Path::GetParent() const
	{
		return Path::GetParent(Data);
	}

	StringView Path::GetFileName(bool Extension) const
	{
		return Path::GetFileName(Data, Extension);
	}

	StringView Path::GetExtension() const
	{
		return Path::GetExtension(Data);
	}

	List<StringView> Path::Split() const
	{
		return Path::Split(Data);
	}

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Operator
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	Path operator+(const Path& A, const Path& B)
	{
		return Path::ConvertStringToPath(Path::Combine(A, B));
	}

	Path operator+(const Path& A, StringView B)
	{
		return Path::ConvertStringToPath(Path::Combine(A, B));
	}

	Path operator+(StringView A, const Path& B)
	{
		return Path::ConvertStringToPath(Path::Combine(A, B));
	}

	Path operator-(const Path& A, uint64 Count)
	{
		return Path::ConvertStringToPath(Path::Previous(A, Count));
	}

	bool operator==(const Path& A, const Path& B)
	{
		return A.ToView() == B.ToView();
	}

	bool operator==(const Path& A, StringView B)
	{
		return A.ToView() == B;
	}

	bool operator==(StringView A, const Path& B)
	{
		return A == B.ToView();
	}

	bool operator!=(const Path& A, const Path& B)
	{
		return A.ToView() != B.ToView();
	}

	bool operator!=(const Path& A, StringView B)
	{
		return A.ToView() != B;
	}

	bool operator!=(StringView A, const Path& B)
	{
		return A != B.ToView();
	}

	bool operator<(const Path& A, const Path& B)
	{
		return A.ToView() < B.ToView();
	}

	bool operator<(const Path& A, StringView B)
	{
		return A.ToView() < B;
	}

	bool operator<(StringView A, const Path& B)
	{
		return A < B.ToView();
	}

	bool operator<=(const Path& A, const Path& B)
	{
		return A.ToView() <= B.ToView();
	}

	bool operator<=(const Path& A, StringView B)
	{
		return A.ToView() <= B;
	}

	bool operator<=(StringView A, const Path& B)
	{
		return A <= B.ToView();
	}

	bool operator>(const Path& A, const Path& B)
	{
		return A.ToView() > B.ToView();
	}

	bool operator>(const Path& A, StringView B)
	{
		return A.ToView() > B;
	}

	bool operator>(StringView A, const Path& B)
	{
		return A > B.ToView();
	}

	bool operator>=(const Path& A, const Path& B)
	{
		return A.ToView() >= B.ToView();
	}

	bool operator>=(const Path& A, StringView B)
	{
		return A.ToView() >= B;
	}

	bool operator>=(StringView A, const Path& B)
	{
		return A >= B.ToView();
	}
}
