#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Containers/Collection.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	class Path
	{
	public:
		enum class Type : uint32
		{
			Invalid, File, Directory
		};

		inline static const String SeparatorDirectory = "/";
		inline static const String SeparatorExtension = ".";
		inline static const String SeparatorPrevious = "..";
		inline static const String SeparatorDrive = ":/";

		NEXUS_FRAMEWORK_API static Path OpenFileDialog(NxFr::StringView Title = "", NxFr::StringView Extension = "", NxFr::StringView Name = "", NxFr::StringView Path = "");
		NEXUS_FRAMEWORK_API static Path GetWorkingDirectory();

		template<typename T>
		static String Combine(const Collection<T>& Elements);
		template<typename T>
		static void Combine(String& Base, const Collection<T>& Elements);
		NEXUS_FRAMEWORK_API static String Combine(StringView Base, StringView Element);
		NEXUS_FRAMEWORK_API static void Combine(String& Base, StringView Element);
		NEXUS_FRAMEWORK_API static String Previous(StringView Path, uint64 Count = 1);
		NEXUS_FRAMEWORK_API static void Previous(String& Path, uint64 Count = 1);

		NEXUS_FRAMEWORK_API static String ChangeFileName(StringView Path, StringView File);
		NEXUS_FRAMEWORK_API static void ChangeFileName(String& Path, StringView File);
		NEXUS_FRAMEWORK_API static String ChangeDirectoryName(StringView Path, StringView Directory);
		NEXUS_FRAMEWORK_API static void ChangeDirectoryName(String& Path, StringView Directory);
		NEXUS_FRAMEWORK_API static String ChangeDirectoryPath(StringView Path, StringView Directory);
		NEXUS_FRAMEWORK_API static void ChangeDirectoryPath(String& Path, StringView Directory);
		NEXUS_FRAMEWORK_API static String ChangeExtension(StringView Path, StringView Extension);
		NEXUS_FRAMEWORK_API static void ChangeExtension(String& Path, StringView Extension);
		NEXUS_FRAMEWORK_API static String ConvertAbsoluteToRelative(StringView Path, StringView Root);
		NEXUS_FRAMEWORK_API static void ConvertAbsoluteToRelative(String& Path, StringView Root);
		NEXUS_FRAMEWORK_API static String ConvertRelativeToAbsolute(StringView Path, StringView Root);
		NEXUS_FRAMEWORK_API static void ConvertRelativeToAbsolute(String& Path, StringView Root);
		NEXUS_FRAMEWORK_API static String Resolve(StringView Path);
		NEXUS_FRAMEWORK_API static void Resolve(String& Path);
		NEXUS_FRAMEWORK_API static String Normalize(StringView Path);
		NEXUS_FRAMEWORK_API static void Normalize(String& Path);

		NEXUS_FRAMEWORK_API static Type GetType(StringView Path);
		NEXUS_FRAMEWORK_API static bool Exist(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsFile(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsDirectory(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsAbsolute(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsRelative(StringView Path);
		NEXUS_FRAMEWORK_API static bool HasExtension(StringView Path, StringView Extension);
		NEXUS_FRAMEWORK_API static StringView GetDrive(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetDirectoryPath(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetDirectoryName(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetParent(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetFileName(StringView Path, bool Extension = false);
		NEXUS_FRAMEWORK_API static StringView GetExtension(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetPathWithoutExtension(StringView Path);
		NEXUS_FRAMEWORK_API static List<StringView> Split(StringView Path);

		NEXUS_FRAMEWORK_API Path(StringView Path = "");
		NEXUS_FRAMEWORK_API Path(const Path& Other);
		NEXUS_FRAMEWORK_API Path(Path&& Other) noexcept;
		NEXUS_FRAMEWORK_API ~Path();
		NEXUS_FRAMEWORK_API static Path ConvertStringToPath(String&& Temp);

		NEXUS_FRAMEWORK_API explicit operator bool () const;
		NEXUS_FRAMEWORK_API operator StringView () const;

		NEXUS_FRAMEWORK_API Path& operator=(StringView Path);
		NEXUS_FRAMEWORK_API Path& operator=(const String& Path);
		NEXUS_FRAMEWORK_API Path& operator=(const Path& Other);
		NEXUS_FRAMEWORK_API Path& operator=(Path&& Other) noexcept;

		NEXUS_FRAMEWORK_API Path& operator +=(const Path& Other);
		NEXUS_FRAMEWORK_API Path& operator +=(StringView Other);
		NEXUS_FRAMEWORK_API Path& operator -=(uint64 Count);

		template<typename T>
		Path& Join(const Collection<T>& Elements);
		NEXUS_FRAMEWORK_API Path& Join(StringView Element);
		NEXUS_FRAMEWORK_API Path& Previous(uint64 Count = 1);

		NEXUS_FRAMEWORK_API Path& ChangeFileName(StringView File);
		NEXUS_FRAMEWORK_API Path& ChangeDirectoryName(StringView Directory);
		NEXUS_FRAMEWORK_API Path& ChangeDirectoryPath(StringView Directory);
		NEXUS_FRAMEWORK_API Path& ChangeExtension(StringView Extension);
		NEXUS_FRAMEWORK_API Path& ConvertAbsoluteToRelative(StringView Root);
		NEXUS_FRAMEWORK_API Path& ConvertRelativeToAbsolute(StringView Root);
		NEXUS_FRAMEWORK_API Path& Resolve();
		NEXUS_FRAMEWORK_API Path& Normalize();

		NEXUS_FRAMEWORK_API Type GetType() const;
		NEXUS_FRAMEWORK_API bool Exist() const;
		NEXUS_FRAMEWORK_API bool IsFile() const;
		NEXUS_FRAMEWORK_API bool IsDirectory() const;
		NEXUS_FRAMEWORK_API bool IsAbsolute() const;
		NEXUS_FRAMEWORK_API bool IsRelative() const;
		NEXUS_FRAMEWORK_API bool HasExtension(StringView Extension) const;
		NEXUS_FRAMEWORK_API StringView GetDrive() const;
		NEXUS_FRAMEWORK_API StringView GetDirectoryPath() const;
		NEXUS_FRAMEWORK_API StringView GetDirectoryName() const;
		NEXUS_FRAMEWORK_API StringView GetParent() const;
		NEXUS_FRAMEWORK_API StringView GetFileName(bool Extension = false) const;
		NEXUS_FRAMEWORK_API StringView GetExtension() const;
		NEXUS_FRAMEWORK_API StringView GetPathWithoutExtension() const;
		NEXUS_FRAMEWORK_API List<StringView> Split() const;

		NEXUS_FRAMEWORK_API const char* C() const { return Data.C(); }
		NEXUS_FRAMEWORK_API bool IsValid() const { return !Data.IsEmpty(); }
		NEXUS_FRAMEWORK_API bool IsEmpty() const { return Data.IsEmpty(); }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Data.GetCount(); }

		String Data;
	};

	NEXUS_FRAMEWORK_API Path operator +(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API Path operator +(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API Path operator +(StringView A, const Path& B);
	NEXUS_FRAMEWORK_API Path operator -(const Path& A, uint64 Count);
	NEXUS_FRAMEWORK_API bool operator ==(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator ==(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API bool operator ==(StringView A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator !=(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator !=(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API bool operator !=(StringView A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator <(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator <(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API bool operator <(StringView A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator <=(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator <=(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API bool operator <=(StringView A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator >(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator >(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API bool operator >(StringView A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator >=(const Path& A, const Path& B);
	NEXUS_FRAMEWORK_API bool operator >=(const Path& A, StringView B);
	NEXUS_FRAMEWORK_API bool operator >=(StringView A, const Path& B);

	template<typename T>
	inline Path& Path::Join(const Collection<T>& Elements)
	{
		Path::Combine(Data, Elements);
		return *this;
	}

	template<typename T>
	inline String Path::Combine(const Collection<T>& Elements)
	{
		String Result = String();
		Path::Combine(Result, Elements);
		return Result;
	}

	template<typename T>
	inline void Path::Combine(String& Base, const Collection<T>& Elements)
	{
		for (auto& It = Elements.Reset(); It != Elements.End(); ++It)
		{
			Base += *It;
			if (!StringUtility::End(*It, SeparatorDirectory) && !IsFile(*It))
			{
				Base += SeparatorDirectory;
			}
		}
	}
}
