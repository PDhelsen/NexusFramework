#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Collection.h"

namespace NxFr
{
	class NX_FRAMEWORK_API Path
	{
	public:
		struct NX_FRAMEWORK_API Info
		{
			Info()
				: Path(), Depth(0),
				Drive(), Folder(), Name(), Extension()
			{
			}

			StringView Path;
			int64 Depth;

			StringView Drive;
			StringView Folder;
			StringView Name;
			StringView Extension;
		};

		inline static const String SeparatorFolder = "/";
		inline static const String SeparatorExtension = ".";
		inline static const String SeparatorPrevious = "..";
		inline static const String SeparatorDrive = ":/";

		static void ShowInExplorer(StringView Path);
		static String OpenFileDialog(StringView Title = "", StringView Extension = "", StringView Name = "", StringView Path = "");
		static String GetWorkingDirectory();
		static bool Exist(StringView Path);
		static void EnsureParent(StringView Path);
		static Info Parse(StringView Path);

		static bool IsAbsolute(StringView Path);
		static bool IsRelative(StringView Path);
		static bool IsSubdirectory(StringView Path, StringView Root);
		static bool IsDirectory(StringView Path);
		static bool IsFile(StringView Path);
		static bool HasDrive(StringView Path);
		static bool HasFolder(StringView Path);
		static bool HasName(StringView Path);
		static bool HasExtension(StringView Path);
		static StringView GetDrive(StringView Path, bool WithSeparator = false);
		static StringView GetFolder(StringView Path, bool WithSeparator = false);
		static StringView GetName(StringView Path, bool WithSeparator = false);
		static StringView GetExtension(StringView Path, bool WithSeparator = false);
		static StringView GetDriveAndFolder(StringView Path, bool WithSeparator = false);
		static StringView GetFolderAndName(StringView Path, bool WithSeparator = false);
		static StringView GetNameAndExtension(StringView Path);
		static StringView GetPathWithoutDrive(StringView Path);
		static StringView GetPathWithoutExtension(StringView Path);
		static List<StringView> Split(StringView Path);
		static int64 Depth(StringView Path);
		static String ChangeDrive(StringView Path, StringView Drive);
		static String ChangeFolder(StringView Path, StringView Folder);
		static String ChangeName(StringView Path, StringView Name);
		static String ChangeExtension(StringView Path, StringView Extension);

		static String Join(StringView Path, StringView Element);
		static void Join(String& Path, StringView Element);
		static String Normalize(StringView Path);
		static void Normalize(String& Path);
		static String Resolve(StringView Path);
		static void Resolve(String& Path);
		static String MakeAbsolute(StringView Path, StringView Root);
		static void MakeAbsolute(String& Path, StringView Root);
		static String MakeRelative(StringView Path, StringView Root);
		static void MakeRelative(String& Path, StringView Root);

		template<typename T>
		static String Combine(const Collection<T>& Elements)
		{
			String Result;
			for (auto It = Elements.Begin(); It != Elements.End(); ++It)
			{
				Join(Result, It);
			}
			return Result;
		}
		template<typename... Args>
		static String Combine(Args&&... args)
		{
			String Result;
			(Join(Result, Forward<Args>(args)), ...);
			return Result;
		}
	};
}
