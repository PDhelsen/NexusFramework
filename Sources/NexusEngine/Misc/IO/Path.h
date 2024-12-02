#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Strings/StringView.h"
#include "Types/Containers/Collection.h"

namespace NxEn
{
	class Path
	{
	public:
		NEXUS_ENGINE_API Path();
		NEXUS_ENGINE_API Path(StringView Path);
		NEXUS_ENGINE_API ~Path();

		NEXUS_ENGINE_API operator StringView () const;
		NEXUS_ENGINE_API bool operator ==(const Path& Other) const;
		NEXUS_ENGINE_API bool operator !=(const Path& Other) const;
		NEXUS_ENGINE_API Path& operator +=(const Path& Other);
		NEXUS_ENGINE_API Path& operator +=(StringView Other);
		NEXUS_ENGINE_API Path& operator -=(const Path& Other);
		NEXUS_ENGINE_API Path& operator -=(StringView Other);

		NEXUS_ENGINE_API Path& ChangeFileName(StringView File);
		NEXUS_ENGINE_API Path& ChangeDirectoryPath(StringView Directory);
		NEXUS_ENGINE_API Path& ChangeExtension(StringView Extension);
		NEXUS_ENGINE_API Path& ConvertAbsoluteToRelative(StringView Root);
		NEXUS_ENGINE_API Path& ConvertRelativeToAbsolute(StringView Root);
		NEXUS_ENGINE_API Path& Resolve();
		NEXUS_ENGINE_API Path& Normalize(bool Directory);

		NEXUS_ENGINE_API bool Exist() const;
		NEXUS_ENGINE_API bool IsFile() const;
		NEXUS_ENGINE_API bool IsDirectory() const;
		NEXUS_ENGINE_API bool IsAbsolute() const;
		NEXUS_ENGINE_API bool IsRelative() const;
		NEXUS_ENGINE_API bool HasExtension(StringView Extension) const;
		NEXUS_ENGINE_API StringView GetFileName(bool Extension = false) const;
		NEXUS_ENGINE_API StringView GetDirectoryPath() const;
		NEXUS_ENGINE_API StringView GetDirectoryName() const;
		NEXUS_ENGINE_API StringView GetExtension() const;
		NEXUS_ENGINE_API StringView GetDrive() const;

		String& GetData() { return Data; };
		String ToString() const { return Data; };
		StringView ToView() const { return Data.ToView(); };
		const char* C() const { return Data.C(); };

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
		NEXUS_ENGINE_API static String Normalize(StringView Path, bool Directory);
		NEXUS_ENGINE_API static void Normalize(String& Path, bool Directory);

		NEXUS_ENGINE_API static bool Exist(StringView Path);
		NEXUS_ENGINE_API static bool IsFile(StringView Path);
		NEXUS_ENGINE_API static bool IsDirectory(StringView Path);
		NEXUS_ENGINE_API static bool IsAbsolute(StringView Path);
		NEXUS_ENGINE_API static bool IsRelative(StringView Path);
		NEXUS_ENGINE_API static bool HasExtension(StringView Path, StringView Extension);
		NEXUS_ENGINE_API static StringView GetFileName(StringView Path, bool Extension = false);
		NEXUS_ENGINE_API static StringView GetDirectoryPath(StringView Path);
		NEXUS_ENGINE_API static StringView GetDirectoryName(StringView Path);
		NEXUS_ENGINE_API static StringView GetExtension(StringView Path);
		NEXUS_ENGINE_API static StringView GetDrive(StringView Path);

		template<typename T>
		static String Combine(const Collection<T>& Elements, bool Directory = false);
		NEXUS_ENGINE_API static String Combine(StringView First, StringView Second, bool Directory = false);

		NEXUS_ENGINE_API static Path GetWorkingDirectory();

	private:
		String Data;
	};

	NEXUS_ENGINE_API bool operator ==(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator ==(StringView A, const Path& B);
	NEXUS_ENGINE_API bool operator !=(const Path& A, StringView B);
	NEXUS_ENGINE_API bool operator !=(StringView A, const Path& B);
	NEXUS_ENGINE_API Path operator +(const Path& A, const Path& B);
	NEXUS_ENGINE_API Path operator +(const Path& A, StringView B);
	NEXUS_ENGINE_API Path operator +(StringView A, const Path& B);
	NEXUS_ENGINE_API Path operator -(const Path& A, const Path& B);
	NEXUS_ENGINE_API Path operator -(const Path& A, StringView B);
	NEXUS_ENGINE_API Path operator -(StringView A, const Path& B);

	template<typename T>
	String Path::Combine(const Collection<T>& Elements, bool Directory)
	{
		uint64 Size = 0;
		for (auto& It = Elements.Current(); It != Elements.End(); ++It)
		{
			Size += It->GetCount();
		}

		uint64 Iter = 1;
		String Result = String(Size + Elements.GetCount());
		for (auto& It = Elements.Reset(); It != Elements.End(); ++It, ++Iter)
		{
			Result += *It;
			if (Iter != Elements.GetCount() || Directory)
			{
				Result += "/";
			}
		}

		return Result;
	}
}
