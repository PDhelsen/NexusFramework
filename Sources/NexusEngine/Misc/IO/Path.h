#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Containers/Collection.h"

namespace NxEn
{
	class Path
	{
	public:
		enum class Type : uint32
		{
			Invalid, File, Directory
		};

		NEXUS_ENGINE_API Path();
		NEXUS_ENGINE_API Path(StringView Path);
		NEXUS_ENGINE_API ~Path();
		NEXUS_ENGINE_API static Path ConvertStringToPath(String&& Temp);

		NEXUS_ENGINE_API explicit operator bool () const;
		NEXUS_ENGINE_API operator StringView () const;
		NEXUS_ENGINE_API Path& operator +=(const Path& Other);
		NEXUS_ENGINE_API Path& operator +=(StringView Other);
		NEXUS_ENGINE_API Path& operator -=(uint64 Count);

		template<typename T>
		Path& Join(const Collection<T>& Elements);
		NEXUS_ENGINE_API Path& Join(StringView Element);
		NEXUS_ENGINE_API Path& Previous(uint64 Count = 1);

		NEXUS_ENGINE_API Path& ChangeFileName(StringView File);
		NEXUS_ENGINE_API Path& ChangeDirectoryPath(StringView Directory);
		NEXUS_ENGINE_API Path& ChangeExtension(StringView Extension);
		NEXUS_ENGINE_API Path& ConvertAbsoluteToRelative(StringView Root);
		NEXUS_ENGINE_API Path& ConvertRelativeToAbsolute(StringView Root);
		NEXUS_ENGINE_API Path& Resolve();
		NEXUS_ENGINE_API Path& Normalize();

		NEXUS_ENGINE_API Type GetType() const;
		NEXUS_ENGINE_API bool Exist() const;
		NEXUS_ENGINE_API bool IsFile() const;
		NEXUS_ENGINE_API bool IsDirectory() const;
		NEXUS_ENGINE_API bool IsAbsolute() const;
		NEXUS_ENGINE_API bool IsRelative() const;
		NEXUS_ENGINE_API bool HasExtension(StringView Extension) const;
		NEXUS_ENGINE_API StringView GetDrive() const;
		NEXUS_ENGINE_API StringView GetDirectoryPath() const;
		NEXUS_ENGINE_API StringView GetDirectoryName() const;
		NEXUS_ENGINE_API StringView GetParent() const;
		NEXUS_ENGINE_API StringView GetFileName(bool Extension = false) const;
		NEXUS_ENGINE_API StringView GetExtension() const;
		NEXUS_ENGINE_API List<StringView> Split() const;

		String ToString() const { return Data; };
		StringView ToView() const { return Data.ToView(); };
		const char* C() const { return Data.C(); };
		bool IsValid() const { return Data != String::Empty; }

		template<typename T>
		static String Combine(const Collection<T>& Elements);
		template<typename T>
		static void Combine(String& Base, const Collection<T>& Elements);
		NEXUS_ENGINE_API static String Combine(StringView Base, StringView Element);
		NEXUS_ENGINE_API static void Combine(String& Base, StringView Element);
		NEXUS_ENGINE_API static String Previous(StringView Path, uint64 Count = 1);
		NEXUS_ENGINE_API static void Previous(String& Path, uint64 Count = 1);

		NEXUS_ENGINE_API static String ChangeFileName(StringView Path, StringView File);
		NEXUS_ENGINE_API static void ChangeFileName(String& Path, StringView File);
		NEXUS_ENGINE_API static String ChangeDirectoryPath(StringView Path, StringView Directory);
		NEXUS_ENGINE_API static void ChangeDirectoryPath(String& Path, StringView Directory);
		NEXUS_ENGINE_API static String ChangeExtension(StringView Path, StringView Extension);
		NEXUS_ENGINE_API static void ChangeExtension(String& Path, StringView Extension);
		NEXUS_ENGINE_API static String ConvertAbsoluteToRelative(StringView Path, StringView Root);
		NEXUS_ENGINE_API static void ConvertAbsoluteToRelative(String& Path, StringView Root);
		NEXUS_ENGINE_API static String ConvertRelativeToAbsolute(StringView Path, StringView Root);
		NEXUS_ENGINE_API static void ConvertRelativeToAbsolute(String& Path, StringView Root);
		NEXUS_ENGINE_API static String Resolve(StringView Path);
		NEXUS_ENGINE_API static void Resolve(String& Path);
		NEXUS_ENGINE_API static String Normalize(StringView Path);
		NEXUS_ENGINE_API static void Normalize(String& Path);

		NEXUS_ENGINE_API static Type GetType(StringView Path);
		NEXUS_ENGINE_API static bool Exist(StringView Path);
		NEXUS_ENGINE_API static bool IsFile(StringView Path);
		NEXUS_ENGINE_API static bool IsDirectory(StringView Path);
		NEXUS_ENGINE_API static bool IsAbsolute(StringView Path);
		NEXUS_ENGINE_API static bool IsRelative(StringView Path);
		NEXUS_ENGINE_API static bool HasExtension(StringView Path, StringView Extension);
		NEXUS_ENGINE_API static StringView GetDrive(StringView Path);
		NEXUS_ENGINE_API static StringView GetDirectoryPath(StringView Path);
		NEXUS_ENGINE_API static StringView GetDirectoryName(StringView Path);
		NEXUS_ENGINE_API static StringView GetParent(StringView Path);
		NEXUS_ENGINE_API static StringView GetFileName(StringView Path, bool Extension = false);
		NEXUS_ENGINE_API static StringView GetExtension(StringView Path);
		NEXUS_ENGINE_API static List<StringView> Split(StringView Path);

		NEXUS_ENGINE_API static Path GetWorkingDirectory();

		NEXUS_ENGINE_API static String SeparatorDirectory;
		NEXUS_ENGINE_API static String SeparatorExtension;

		String Data;
	};

	NEXUS_ENGINE_API Path operator +(const Path& A, const Path& B);
	NEXUS_ENGINE_API Path operator +(const Path& A, StringView B);
	NEXUS_ENGINE_API Path operator +(StringView A, const Path& B);
	NEXUS_ENGINE_API Path operator -(const Path& A, uint64 Count);
	NEXUS_ENGINE_API bool operator ==(const Path& A, const Path& B);
	NEXUS_ENGINE_API bool operator ==(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator ==(StringView A, const Path& B);
	NEXUS_ENGINE_API bool operator !=(const Path& A, const Path& B);
	NEXUS_ENGINE_API bool operator !=(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator !=(StringView A, const Path& B);
	NEXUS_ENGINE_API bool operator <(const Path& A, const Path& B);
	NEXUS_ENGINE_API bool operator <(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator <(StringView A, const Path& B);
	NEXUS_ENGINE_API bool operator <=(const Path& A, const Path& B);
	NEXUS_ENGINE_API bool operator <=(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator <=(StringView A, const Path& B);
	NEXUS_ENGINE_API bool operator >(const Path& A, const Path& B);
	NEXUS_ENGINE_API bool operator >(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator >(StringView A, const Path& B);
	NEXUS_ENGINE_API bool operator >=(const Path& A, const Path& B);
	NEXUS_ENGINE_API bool operator >=(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator >=(StringView A, const Path& B);

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
			if (!(*It).End(SeparatorDirectory) && !IsFile(*It))
			{
				Base += SeparatorDirectory;
			}
		}
	}
}
