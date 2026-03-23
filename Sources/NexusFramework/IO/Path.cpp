#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/Path.h"

#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
#pragma region Info

	Path::Info::Info(NxFr::StringView Data)
		: Path(Data), Depth(0),
		Drive(), Folder(), Name(), Extension()
	{
		const char* Pointer = Path.C();

		bool StartBySeparatorDrive = StringCApi::Compare(Path.C(), SeparatorFolder.C(), SeparatorFolder.GetCount()) == 0;
		if (StartBySeparatorDrive)
		{
			Pointer += SeparatorFolder.GetCount();
		}

		while (*Pointer != StringCApi::NullChar)
		{
			if (StringCApi::Compare(Pointer, SeparatorPrevious.C(), SeparatorPrevious.GetCount()) == 0)
			{
				Depth--;

				Pointer += SeparatorPrevious.GetCount();
				continue;
			}
			else if (StringCApi::Compare(Pointer, SeparatorDrive.C(), SeparatorDrive.GetCount()) == 0)
			{
				NEXUS_ASSERT(Drive.IsEmpty(), Default, "Two drive separator has been found. That is not supported");

				uint64 Position = Pointer - Path.C();
				Drive = Path.Substring(0, Position);

				Pointer += SeparatorDrive.GetCount();
				continue;
			}
			else if (StringCApi::Compare(Pointer, SeparatorFolder.C(), SeparatorFolder.GetCount()) == 0)
			{
				uint64 Position = Pointer - Path.C();
				uint64 Offset = !Drive.IsEmpty() ? Drive.GetCount() + SeparatorDrive.GetCount() : StartBySeparatorDrive ? 1 : 0;
				Folder = Path.Substring(Offset, Position >= Offset ? Position - Offset : 0);
				Name = StringUtility::Empty;
				Depth++;

				Pointer += SeparatorFolder.GetCount();
				continue;
			}
			else if (StringCApi::Compare(Pointer, SeparatorExtension.C(), SeparatorExtension.GetCount()) == 0)
			{
				NEXUS_ASSERT(Extension.IsEmpty(), Default, "Two extension separators has been found. That is not supported");

				uint64 Position = Pointer - Path.C();
				uint64 Offset = SeparatorExtension.GetCount();
				Name = Path.Substring(Name.C() - Path.C(), Pointer - Name.C());
				Extension = Path.Substring(Position + Offset, Path.GetCount() - (Position + Offset));

				break;
			}
			else if (Name.IsEmpty())
			{
				Name = Pointer;
			}

			Pointer++;
		}
	}

	bool Path::Info::Has(bool HasDrive, bool HasFolder, bool HasName, bool HasExtension) const
	{
		bool Result = true;
		if (HasDrive)
		{
			Result &= !Drive.IsEmpty();
		}
		if (HasFolder)
		{
			Result &= !Folder.IsEmpty();
		}
		if (HasName)
		{
			Result &= !Name.IsEmpty();
		}
		if (HasExtension)
		{
			Result &= !Extension.IsEmpty();
		}
		return Result;
	}

	StringView Path::Info::Get(bool GetDrive, bool GetFolder, bool GetName, bool GetExtension) const
	{
		uint64 Offset = 0;
		uint64 Count = 0;
		if (GetDrive && !Drive.IsEmpty())
		{
			Offset = 0;
			Count = Drive.GetCount();
		}
		if (GetFolder && !Folder.IsEmpty())
		{
			Offset = Count != 0 ? Offset : Folder.C() - Path.C();
			Count = (Folder.C() + Folder.GetCount()) - (Path.C() + Offset);
		}
		if (GetName && !Name.IsEmpty())
		{
			Offset = Count != 0 ? Offset : Name.C() - Path.C();
			Count = (Name.C() + Name.GetCount()) - (Path.C() + Offset);
		}
		if (GetExtension && !Extension.IsEmpty())
		{
			Offset = Count != 0 ? Offset : Extension.C() - Path.C();
			Count = (Extension.C() + Extension.GetCount()) - (Path.C() + Offset);
		}
		return StringView(Path.C(), Offset, Count);
	}

	void Path::Info::Change(StringView ChangeDrive, StringView ChangeFolder, StringView ChangeName, StringView ChangeExtension, String& Buffer) const
	{
		Buffer.Clear();
		if (!Drive.IsEmpty() || !ChangeDrive.IsEmpty())
		{
			Buffer += !ChangeDrive.IsEmpty() ? ChangeDrive : Drive;
			Buffer += SeparatorDrive;
		}
		if (!Folder.IsEmpty() || !ChangeFolder.IsEmpty())
		{
			Buffer += !ChangeFolder.IsEmpty() ? ChangeFolder : Folder;
			Buffer += SeparatorFolder;
		}
		if (!Name.IsEmpty() || !ChangeName.IsEmpty())
		{
			Buffer += !ChangeName.IsEmpty() ? ChangeName : Name;
		}
		if (!Extension.IsEmpty() || !ChangeExtension.IsEmpty())
		{
			Buffer += SeparatorExtension;
			Buffer += !ChangeExtension.IsEmpty() ? ChangeExtension : Extension;
		}
	}

	List<StringView> Path::Info::Split() const
	{
		List<StringView> Result;
		if (!Drive.IsEmpty())
		{
			Result.Append(Drive);
		}
		if (!Folder.IsEmpty())
		{
			const char* Pointer = Folder.C();
			const char* Start = Pointer;
			uint64 Count = Folder.GetCount();

			while (Count-- > 0)
			{
				if (StringCApi::Compare(Pointer, SeparatorFolder.C(), SeparatorFolder.GetCount()) == 0)
				{
					Result.Append(StringView(Folder.C(), Start - Folder.C(), Pointer - Start));
				}
			}
			Result.Append(StringView(Folder.C(), Start - Folder.C(), Pointer - Start));
		}
		if (!Name.IsEmpty())
		{
			Result.Append(Name);
		}
		if (!Extension.IsEmpty())
		{
			Result.Append(Extension);
		}
		return Result;
	}

#pragma endregion

#pragma region Platform

	void Path::ShowInExplorer(NxFr::StringView Path)
	{
		Platform::GetInstance()->OpenExplorer(Path);
	}

	String Path::OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path)
	{
		return Normalize(Platform::GetInstance()->OpenFileDialog(Title, Extension, Name, Path));
	}

	String Path::GetWorkingDirectory()
	{
		return Normalize(Platform::GetInstance()->GetWorkingDirectory() + Path::SeparatorFolder);
	}

#pragma endregion

#pragma region Computation

	String Path::Join(StringView Path, StringView Element)
	{
		String Result = Path;
		if (!Path.IsEmpty() && !StringUtility::End(Path, SeparatorFolder))
		{
			Result += SeparatorFolder;
		}
		Result += Element;
		return Result;
	}

	String Path::Normalize(StringView Path)
	{
		String Result = Path;
		Result = StringUtility::Replace(Result, "\\", SeparatorFolder);
		Result = StringUtility::Replace(Result, "//", SeparatorFolder);
		return Result;
	}

	String Path::Resolve(StringView Path)
	{
		const char* Pointer = Path.C();
		const char* Start = Pointer;
		List<StringView> Parts;

		while (*Pointer != StringCApi::NullChar)
		{
			if (StringCApi::Compare(Pointer, SeparatorFolder.C(), SeparatorFolder.GetCount()) == 0)
			{
				StringView Part = StringView(Start, Pointer - Start);
				if (Part == SeparatorPrevious)
				{
					if (!Parts.IsEmpty())
					{
						Parts.RemoveLast();
						Parts.RemoveLast();
					}
				}
				else
				{
					if (!Parts.IsEmpty())
					{
						Parts.Append(SeparatorFolder);
					}
					Parts.Append(Part);
				}

				Pointer++;
				Start = Pointer;
			}
			else
			{
				Pointer++;
			}
		}

		if (Pointer != Start)
		{
			StringView Part = StringView(Start, Pointer - Start);
			if (!Parts.IsEmpty())
			{
				Parts.Append(SeparatorFolder);
			}
			Parts.Append(Part);
		}

		String Result = Path.GetCount();
		for (auto& It : Parts)
		{
			Result += It;
		}
		return Result;
	}

	String Path::MakeRelative(StringView Path, StringView Root)
	{
		List<StringView> PartsPath = StringUtility::SplitAll(Path, SeparatorFolder);
		List<StringView> PartsRoot = StringUtility::SplitAll(Root, SeparatorFolder);

		uint64 Index = 0;
		while (Index < PartsPath.GetCount() && Index < PartsRoot.GetCount() && PartsPath[Index] == PartsRoot[Index])
		{
			Index++;
		}

		String Result;
		for (uint64 I = Index; I < PartsRoot.GetCount(); ++I)
		{
			if (!Result.IsEmpty())
			{
				Result += SeparatorFolder;
			}
			Result += SeparatorPrevious;
		}
		for (uint64 I = Index; I < PartsPath.GetCount(); ++I)
		{
			if (!Result.IsEmpty())
			{
				Result += SeparatorFolder;
			}
			Result += PartsPath[I];
		}
		return Result;
	}

	String Path::MakeAbsolute(StringView Path, StringView Root)
	{
		String Result = Join(Root, Path);
		Result = Resolve(Result);
		return Result;
	}

	bool Path::Exist(StringView Path)
	{
		return Platform::GetInstance()->GetPathType(Path) != Platform::PathType::None;
	}

#pragma endregion

#pragma region Infos

	bool Path::IsAbsolute(StringView Path)
	{
		Info Infos(Path);
		return !Infos.Drive.IsEmpty();
	}

	bool Path::IsRelative(StringView Path)
	{
		Info Infos(Path);
		return Infos.Drive.IsEmpty();
	}

	bool Path::IsSubdirectory(StringView Path, StringView Root)
	{
		Info InfosPath(Path);
		Info InfosRoot(Root);
		return InfosPath.Drive == InfosRoot.Drive && StringUtility::Start(InfosPath.Folder, InfosRoot.Folder);
	}

	bool Path::IsDirectory(StringView Path)
	{
		Info Infos(Path);
		return Infos.Name.IsEmpty() && Infos.Extension.IsEmpty();
	}

	bool Path::IsFile(StringView Path)
	{
		Info Infos(Path);
		return !Infos.Name.IsEmpty() && !Infos.Extension.IsEmpty();
	}

	bool Path::Has(StringView Path, bool HasDrive, bool HasFolder, bool HasName, bool HasExtension)
	{
		return Info(Path).Has(HasDrive, HasFolder, HasName, HasExtension);
	}

	bool Path::HasDrive(StringView Path)
	{
		return Info(Path).Has(true, false, false, false);
	}

	bool Path::HasFolder(StringView Path)
	{
		return Info(Path).Has(false, true, false, false);
	}

	bool Path::HasName(StringView Path)
	{
		return Info(Path).Has(false, false, true, false);
	}

	bool Path::HasExtension(StringView Path)
	{
		return Info(Path).Has(false, false, false, true);
	}

	StringView Path::Get(StringView Path, bool GetDrive, bool GetFolder, bool GetName, bool GetExtension)
	{
		return Info(Path).Get(GetDrive, GetFolder, GetName, GetExtension);
	}

	StringView Path::GetDrive(StringView Path)
	{
		return Info(Path).Get(true, false, false, false);
	}

	StringView Path::GetFolder(StringView Path)
	{
		return Info(Path).Get(false, true, false, false);
	}

	StringView Path::GetName(StringView Path)
	{
		return Info(Path).Get(false, false, true, false);
	}

	StringView Path::GetExtension(StringView Path)
	{
		return Info(Path).Get(false, false, false, true);
	}

	StringView Path::GetDriveAndFolder(StringView Path)
	{
		return Info(Path).Get(true, true, false, false);
	}

	StringView Path::GetFolderAndName(StringView Path)
	{
		return Info(Path).Get(false, true, true, false);
	}

	StringView Path::GetNameAndExtension(StringView Path)
	{
		return Info(Path).Get(false, false, true, true);
	}

	StringView Path::GetPathWithoutDrive(StringView Path)
	{
		return Info(Path).Get(false, true, true, true);
	}

	StringView Path::GetPathWithoutExtension(StringView Path)
	{
		return Info(Path).Get(true, true, true, false);
	}

	String Path::Change(StringView Path, StringView ChangeDrive, StringView ChangeFolder, StringView ChangeName, StringView ChangeExtension)
	{
		String Result;
		Info(Path).Change(ChangeDrive, ChangeFolder, ChangeName, ChangeExtension, Result);
		return Result;
	}

	String Path::ChangeDrive(StringView Path, StringView Drive)
	{
		String Result;
		Info(Path).Change(Drive, "", "", "", Result);
		return Result;
	}

	String Path::ChangeFolder(StringView Path, StringView Folder)
	{
		String Result;
		Info(Path).Change("", Folder, "", "", Result);
		return Result;
	}

	String Path::ChangeName(StringView Path, StringView Name)
	{
		String Result;
		Info(Path).Change("", "", Name, "", Result);
		return Result;
	}

	String Path::ChangeExtension(StringView Path, StringView Extension)
	{
		String Result;
		Info(Path).Change("", "", "", Extension, Result);
		return Result;
	}

	List<StringView> Path::Split(StringView Path)
	{
		return Info(Path).Split();
	}

	int64 Path::Depth(StringView Path)
	{
		return Info(Path).Depth;
	}

#pragma endregion

}
