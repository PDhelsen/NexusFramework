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
		struct Info
		{
			Info()
				: Path(), Depth(0),
				Drive(), Folder(), Name(), Extension()
			{
			}

			NxFr::StringView Path;
			int64 Depth;

			NxFr::StringView Drive;
			NxFr::StringView Folder;
			NxFr::StringView Name;
			NxFr::StringView Extension;
		};

		inline static const String SeparatorFolder = "/";
		inline static const String SeparatorExtension = ".";
		inline static const String SeparatorPrevious = "..";
		inline static const String SeparatorDrive = ":/";

		NEXUS_FRAMEWORK_API static void ShowInExplorer(NxFr::StringView Path);
		NEXUS_FRAMEWORK_API static String OpenFileDialog(NxFr::StringView Title = "", NxFr::StringView Extension = "", NxFr::StringView Name = "", NxFr::StringView Path = "");
		NEXUS_FRAMEWORK_API static String GetWorkingDirectory();
		NEXUS_FRAMEWORK_API static bool Exist(StringView Path);
		NEXUS_FRAMEWORK_API static Info Parse(StringView Path);

		NEXUS_FRAMEWORK_API static bool IsAbsolute(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsRelative(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsSubdirectory(StringView Path, StringView Root);
		NEXUS_FRAMEWORK_API static bool IsDirectory(StringView Path);
		NEXUS_FRAMEWORK_API static bool IsFile(StringView Path);
		NEXUS_FRAMEWORK_API static bool Has(StringView Path, bool HasDrive, bool HasFolder, bool HasName, bool HasExtension);
		NEXUS_FRAMEWORK_API static bool HasDrive(StringView Path);
		NEXUS_FRAMEWORK_API static bool HasFolder(StringView Path);
		NEXUS_FRAMEWORK_API static bool HasName(StringView Path);
		NEXUS_FRAMEWORK_API static bool HasExtension(StringView Path);
		NEXUS_FRAMEWORK_API static StringView Get(StringView Path, bool GetDrive, bool GetFolder, bool GetName, bool GetExtension);
		NEXUS_FRAMEWORK_API static StringView GetDrive(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetFolder(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetName(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetExtension(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetDriveAndFolder(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetFolderAndName(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetNameAndExtension(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetPathWithoutDrive(StringView Path);
		NEXUS_FRAMEWORK_API static StringView GetPathWithoutExtension(StringView Path);
		NEXUS_FRAMEWORK_API static List<StringView> Split(StringView Path);
		NEXUS_FRAMEWORK_API static int64 Depth(StringView Path);
		NEXUS_FRAMEWORK_API static String Change(StringView Path, StringView ChangeDrive, StringView ChangeFolder, StringView ChangeName, StringView ChangeExtension);
		NEXUS_FRAMEWORK_API static String ChangeDrive(StringView Path, StringView Drive);
		NEXUS_FRAMEWORK_API static String ChangeFolder(StringView Path, StringView Folder);
		NEXUS_FRAMEWORK_API static String ChangeName(StringView Path, StringView Name);
		NEXUS_FRAMEWORK_API static String ChangeExtension(StringView Path, StringView Extension);
		NEXUS_FRAMEWORK_API static String MakeRelative(StringView Path, StringView Root);
		NEXUS_FRAMEWORK_API static String MakeAbsolute(StringView Path, StringView Root);
		NEXUS_FRAMEWORK_API static String Resolve(StringView Path);
		NEXUS_FRAMEWORK_API static String Normalize(StringView Path);
		NEXUS_FRAMEWORK_API static String Join(StringView Path, StringView Element);

		template<typename T>
		static String Combine(const Collection<T>& Elements);
		template<typename... Args>
		static String Combine(Args&&... args);
	};

	template<typename T>
	inline String Path::Combine(const Collection<T>& Elements)
	{
		String Result;
		for (auto& It = Elements.Current(); It != Elements.End(); ++It)
		{
			Result = Join(Result, It);
		}
		return Result;
	}

	template<typename ...Args>
	inline String Path::Combine(Args&&... args)
	{
		String Result;
		((Result = Join(Result, Forward<Args>(args))), ...);
		return Result;
	}
}
