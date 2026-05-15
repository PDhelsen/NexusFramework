#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/Path.h"

#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/IO/Directory.h"

namespace NxFr
{
#pragma region Logic

	static Path::Info Parse_Impl(StringView Path)
	{
		Path::Info Infos;

		Infos.Path = Path.C();
		if (Infos.Path.IsEmpty())
		{
			return Infos;
		}

		const char* Pointer = Infos.Path.C();
		const char* End = Infos.Path.C() + Infos.Path.GetCount();

		bool StartBySeparatorDrive = StringCApi::Compare(Pointer, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) == 0;
		bool EndBySeparartorDrive = StringCApi::Compare(End - Path::SeparatorFolder.GetCount(), Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) == 0;
		if (StartBySeparatorDrive)
		{
			Pointer += Path::SeparatorFolder.GetCount();
		}
		if (EndBySeparartorDrive)
		{
			End -= Path::SeparatorFolder.GetCount();
		}

		while (Pointer != End)
		{
			if (StringCApi::Compare(Pointer, Path::SeparatorPrevious.C(), Path::SeparatorPrevious.GetCount()) == 0)
			{
				Infos.Depth--;

				Pointer += Path::SeparatorPrevious.GetCount();
				continue;
			}
			else if (StringCApi::Compare(Pointer, Path::SeparatorDrive.C(), Path::SeparatorDrive.GetCount()) == 0)
			{
				NEXUS_ASSERT(Infos.Drive.IsEmpty(), Default, "Two drive separator has been found. That is not supported");

				uint64 Position = Pointer - Infos.Path.C();
				Infos.Drive = Infos.Path.Substring(0, Position);

				Pointer += Path::SeparatorDrive.GetCount();
				continue;
			}
			else if (StringCApi::Compare(Pointer, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) == 0)
			{
				uint64 Position = Pointer - Infos.Path.C();
				uint64 OffsetFolder = !Infos.Drive.IsEmpty() ? Infos.Drive.GetCount() + Path::SeparatorDrive.GetCount() : StartBySeparatorDrive ? 1 : 0;
				uint64 OffsetName = Path::SeparatorFolder.GetCount();
				Infos.Folder = Infos.Path.Substring(OffsetFolder, Position - OffsetFolder);
				Infos.Name = Infos.Path.Substring(Position + OffsetName, End - (Pointer + OffsetName));
				Infos.Depth++;

				Pointer += Path::SeparatorFolder.GetCount();
				continue;
			}
			else if (StringCApi::Compare(Pointer, Path::SeparatorExtension.C(), Path::SeparatorExtension.GetCount()) == 0)
			{
				NEXUS_ASSERT(Infos.Extension.IsEmpty(), Default, "Two extension separators has been found. That is not supported");

				uint64 Position = Pointer - Infos.Path.C();
				uint64 Offset = Path::SeparatorExtension.GetCount();
				Infos.Name = Infos.Path.Substring(Infos.Name.C() - Infos.Path.C(), Pointer - Infos.Name.C());
				Infos.Extension = Infos.Path.Substring(Position + Offset, End - (Pointer + Offset));

				break;
			}
			else if (Infos.Name.IsEmpty())
			{
				uint64 Position = Pointer - Infos.Path.C();
				Infos.Name = Infos.Path.Substring(Position, End - Pointer);
			}

			Pointer++;
		}

		return Infos;
	}

	static bool Has_Impl(const Path::Info& Infos, bool HasDrive, bool HasFolder, bool HasName, bool HasExtension)
	{
		bool Result = true;
		if (HasDrive)
		{
			Result &= !Infos.Drive.IsEmpty();
		}
		if (HasFolder)
		{
			Result &= !Infos.Folder.IsEmpty();
		}
		if (HasName)
		{
			Result &= !Infos.Name.IsEmpty();
		}
		if (HasExtension)
		{
			Result &= !Infos.Extension.IsEmpty();
		}
		return Result;
	}

	static StringView Get_Impl(const Path::Info& Infos, bool GetDrive, bool GetFolder, bool GetName, bool GetExtension)
	{
		uint64 Offset = 0;
		uint64 Count = 0;
		if (GetDrive && !Infos.Drive.IsEmpty())
		{
			Offset = 0;
			Count = Infos.Drive.GetCount();
		}
		if (GetFolder && !Infos.Folder.IsEmpty())
		{
			Offset = Count != 0 ? Offset : Infos.Folder.C() - Infos.Path.C();
			Count = (Infos.Folder.C() + Infos.Folder.GetCount()) - (Infos.Path.C() + Offset);
		}
		if (GetName && !Infos.Name.IsEmpty())
		{
			Offset = Count != 0 ? Offset : Infos.Name.C() - Infos.Path.C();
			Count = (Infos.Name.C() + Infos.Name.GetCount()) - (Infos.Path.C() + Offset);
		}
		if (GetExtension && !Infos.Extension.IsEmpty())
		{
			Offset = Count != 0 ? Offset : Infos.Extension.C() - Infos.Path.C();
			Count = (Infos.Extension.C() + Infos.Extension.GetCount()) - (Infos.Path.C() + Offset);
		}
		return StringView(Infos.Path.C(), Offset, Count);
	}

	static void Change_Impl(const Path::Info& Infos, String& Buffer, StringView ChangeDrive, StringView ChangeFolder, StringView ChangeName, StringView ChangeExtension)
	{
		Buffer.Clear();
		if (!Infos.Drive.IsEmpty() || !ChangeDrive.IsEmpty())
		{
			Buffer += !ChangeDrive.IsEmpty() ? ChangeDrive : Infos.Drive;
			Buffer += Path::SeparatorDrive;
		}
		if (!Infos.Folder.IsEmpty() || !ChangeFolder.IsEmpty())
		{
			Buffer += !ChangeFolder.IsEmpty() ? ChangeFolder : Infos.Folder;
			Buffer += Path::SeparatorFolder;
		}
		if (!Infos.Name.IsEmpty() || !ChangeName.IsEmpty())
		{
			Buffer += !ChangeName.IsEmpty() ? ChangeName : Infos.Name;
		}
		if (!Infos.Extension.IsEmpty() || !ChangeExtension.IsEmpty())
		{
			Buffer += Path::SeparatorExtension;
			Buffer += !ChangeExtension.IsEmpty() ? ChangeExtension : Infos.Extension;
		}
	}

	static List<StringView> Split_Impl(const Path::Info& Infos)
	{
		List<StringView> Result;
		if (!Infos.Drive.IsEmpty())
		{
			Result.Append(Infos.Drive);
		}
		if (!Infos.Folder.IsEmpty())
		{
			const char* Pointer = Infos.Folder.C();
			const char* Start = Pointer;
			uint64 Count = 0;

			while (Count++ < Infos.Folder.GetCount())
			{
				if (StringCApi::Compare(Pointer, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) == 0)
				{
					Result.Append(StringView(Infos.Folder.C(), Start - Infos.Folder.C(), Pointer - Start));
					Pointer++;
					Start = Pointer;
					continue;
				}

				Pointer++;
			}
			Result.Append(StringView(Infos.Folder.C(), Start - Infos.Folder.C(), Pointer - Start));
		}
		if (!Infos.Name.IsEmpty())
		{
			Result.Append(Infos.Name);
		}
		if (!Infos.Extension.IsEmpty())
		{
			Result.Append(Infos.Extension);
		}
		return Result;
	}

	static void Join_Impl(String& Path, StringView Element)
	{
		if (!Path.IsEmpty() && !StringUtility::End(Path, Path::SeparatorFolder))
		{
			Path += Path::SeparatorFolder;
		}
		Path += Element;
	}

	static void Normalize_Impl(String& Path)
	{
		Path = StringUtility::Replace(Path, "\\", Path::SeparatorFolder);
		Path = StringUtility::Replace(Path, "//", Path::SeparatorFolder);
	}

	static void Resolve_Impl(String& Path)
	{
		const char* Read = Path.Characters();
		char* Write = Path.Characters();
		uint64 Cursor = 0;

		while (Cursor < Path.GetCount())
		{
			const char* Offset = Read;
			while (Cursor < Path.GetCount() && StringCApi::Compare(Read, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) != 0)
			{
				Read++;
				Cursor++;
			}

			if (StringCApi::Compare(Offset, Path::SeparatorPrevious.C(), Path::SeparatorPrevious.GetCount()) == 0)
			{
				if (Write != Path.C() && StringCApi::Compare(Write, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) == 0)
				{
					Write--;
				}

				while (Write != Path.C() && StringCApi::Compare(Write, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) != 0)
				{
					Write--;
				}
			}
			else
			{
				if (Write != Path.C())
				{
					*Write = *Path::SeparatorFolder.C();
					Write++;
				}

				uint64 Length = Read - Offset;
				for (uint64 Count = 0; Count < Length; ++Count)
				{
					*Write = *Offset;
					Write++;
					Offset++;
				}
			}

			Read++;
			Cursor++;
		}

		if (StringUtility::End(Path, Path::SeparatorFolder) && StringCApi::Compare(Write, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) != 0)
		{
			*Write = *Path::SeparatorFolder.C();
			Write++;
		}

		Path.Terminate(Write - Path.C());
	}

	static void Relativize_Impl(String& Path, StringView Root)
	{
		uint64 Count = 0;
		uint64 Common = StringCApi::Common(Path.C(), Root.C());

		const char* Read = Root.C() + Common;
		while (*Read != StringCApi::NullChar)
		{
			Count++;
			while (*Read != StringCApi::NullChar && StringCApi::Compare(Read, Path::SeparatorFolder.C(), Path::SeparatorFolder.GetCount()) != 0)
			{
				Read++;
			}
		}

		NEXUS_ASSERT(Common >= Count * (Path::SeparatorPrevious.GetCount() + Path::SeparatorFolder.GetCount()), Default, "Not enough space");

		char* Write = Path.Characters();
		for (uint64 Index = 0; Index < Count; ++Index)
		{
			*Write++ = *".";
			*Write++ = *".";
			*Write++ = *"/";
		}

		Read = Path.C() + Common;
		while (*Read != StringCApi::NullChar)
		{
			*Write = *Read;
			Write++;
			Read++;
		}

		Path.Terminate(Write - Path.C());
	}

#pragma endregion

#pragma region Global

	void Path::ShowInExplorer(StringView Path)
	{
		Globals::PlatformTarget->OpenExplorer(Path);
	}

	String Path::OpenFileDialog(StringView Title, StringView Extension, StringView Name, StringView Path)
	{
		return Normalize(Globals::PlatformTarget->OpenFileDialog(Title, Extension, Name, Path));
	}

	String Path::GetWorkingDirectory()
	{
		return Globals::PlatformTarget->GetWorkingDirectory();
	}

	bool Path::Exist(StringView Path)
	{
		return Globals::PlatformTarget->GetPathType(Path) != Platform::PathType::None;
	}

	void Path::EnsureParent(StringView Path)
	{
		String Parent = Path::GetDriveAndFolder(Path);
		if (Parent.IsEmpty() || Exist(Path))
		{
			return;
		}

		EnsureParent(Parent);
		Directory(Parent).Create();
	}

	Path::Info Path::Parse(StringView Path)
	{
		return Parse_Impl(Path);
	}

#pragma endregion

#pragma region Query

	bool Path::IsAbsolute(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return !Infos.Drive.IsEmpty();
	}

	bool Path::IsRelative(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Infos.Drive.IsEmpty();
	}

	bool Path::IsSubdirectory(StringView Path, StringView Root)
	{
		Path::Info InfosPath = Parse_Impl(Path);
		Path::Info InfosRoot = Parse_Impl(Root);
		return InfosPath.Drive == InfosRoot.Drive && StringUtility::Start(InfosPath.Folder, InfosRoot.Folder);
	}

	bool Path::IsDirectory(StringView Path)
	{
		return StringUtility::End(Path, SeparatorFolder);
	}

	bool Path::IsFile(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return !Infos.Extension.IsEmpty();
	}

	bool Path::Has(StringView Path, bool HasDrive, bool HasFolder, bool HasName, bool HasExtension)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Has_Impl(Infos, HasDrive, HasFolder, HasName, HasExtension);
	}

	bool Path::HasDrive(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Has_Impl(Infos, true, false, false, false);
	}

	bool Path::HasFolder(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Has_Impl(Infos, false, true, false, false);
	}

	bool Path::HasName(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Has_Impl(Infos, false, false, true, false);
	}

	bool Path::HasExtension(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Has_Impl(Infos, false, false, false, true);
	}

	StringView Path::Get(StringView Path, bool GetDrive, bool GetFolder, bool GetName, bool GetExtension)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, GetDrive, GetFolder, GetName, GetExtension);
	}

	StringView Path::GetDrive(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, true, false, false, false);
	}

	StringView Path::GetFolder(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, false, true, false, false);
	}

	StringView Path::GetName(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, false, false, true, false);
	}

	StringView Path::GetExtension(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, false, false, false, true);
	}

	StringView Path::GetDriveAndFolder(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, true, true, false, false);
	}

	StringView Path::GetFolderAndName(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, false, true, true, false);
	}

	StringView Path::GetNameAndExtension(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, false, false, true, true);
	}

	StringView Path::GetPathWithoutDrive(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, false, true, true, true);
	}

	StringView Path::GetPathWithoutExtension(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Get_Impl(Infos, true, true, true, false);
	}

	List<StringView> Path::Split(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Split_Impl(Infos);
	}

	int64 Path::Depth(StringView Path)
	{
		Path::Info Infos = Parse_Impl(Path);
		return Infos.Depth;
	}

	String Path::Change(StringView Path, StringView ChangeDrive, StringView ChangeFolder, StringView ChangeName, StringView ChangeExtension)
	{
		Path::Info Infos = Parse_Impl(Path);
		String Result;
		Change_Impl(Infos, Result, ChangeDrive, ChangeFolder, ChangeName, ChangeExtension);
		return Result;
	}

	String Path::ChangeDrive(StringView Path, StringView Drive)
	{
		Path::Info Infos = Parse_Impl(Path);
		String Result;
		Change_Impl(Infos, Result, Drive, "", "", "");
		return Result;
	}

	String Path::ChangeFolder(StringView Path, StringView Folder)
	{
		Path::Info Infos = Parse_Impl(Path);
		String Result;
		Change_Impl(Infos, Result, "", Folder, "", "");
		return Result;
	}

	String Path::ChangeName(StringView Path, StringView Name)
	{
		Path::Info Infos = Parse_Impl(Path);
		String Result;
		Change_Impl(Infos, Result, "", "", Name, "");
		return Result;
	}

	String Path::ChangeExtension(StringView Path, StringView Extension)
	{
		Path::Info Infos = Parse_Impl(Path);
		String Result;
		Change_Impl(Infos, Result, "", "", "", Extension);
		return Result;
	}

#pragma endregion

#pragma region Manipulation

	String Path::Join(StringView Path, StringView Element)
	{
		String Result = Path;
		Join(Result, Element);
		return Result;
	}

	void Path::Join(String& Path, StringView Element)
	{
		Join_Impl(Path, Element);
	}

	String Path::Normalize(StringView Path)
	{
		String Result = Path;
		Normalize(Result);
		return Result;
	}

	void Path::Normalize(String& Path)
	{
		Normalize_Impl(Path);
	}

	String Path::Resolve(StringView Path)
	{
		String Result = Path;
		Resolve(Result);
		return Result;
	}

	void Path::Resolve(String& Path)
	{
		Resolve_Impl(Path);
	}

	String Path::MakeAbsolute(StringView Path, StringView Root)
	{
		String Result = Root;
		Join_Impl(Result, Path);
		Resolve_Impl(Result);
		return Result;
	}

	void Path::MakeAbsolute(String& Path, StringView Root)
	{
		Path = Join(Root, Path);
		Resolve_Impl(Path);
	}

	String Path::MakeRelative(StringView Path, StringView Root)
	{
		String Result = Path;
		MakeRelative(Result, Root);
		return Result;
	}

	void Path::MakeRelative(String& Path, StringView Root)
	{
		Relativize_Impl(Path, Root);
	}

#pragma endregion

}
