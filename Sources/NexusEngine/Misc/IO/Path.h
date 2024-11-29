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
		NEXUS_ENGINE_API static String ChangeFileName(StringView Path, StringView FileName);
		NEXUS_ENGINE_API static String ChangeDirectoryPath(StringView Path, StringView DirectoryPath);
		NEXUS_ENGINE_API static String ChangeExtension(StringView Path, StringView Extension);
		NEXUS_ENGINE_API static String ConvertAbsoluteToRelative(StringView Path, StringView Root);
		NEXUS_ENGINE_API static String ConvertRelativeToAbsolute(StringView Path, StringView Root);
		NEXUS_ENGINE_API static String Resolve(StringView Path);
		NEXUS_ENGINE_API static String Normalize(StringView Path, bool Directory);

		NEXUS_ENGINE_API static String GetWorkingDirectory();
	};

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
