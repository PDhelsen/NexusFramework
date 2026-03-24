#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	uint64 Data[10] = { 10,11,12,13,14,15,16,17,18,19 };
	NxFr::String Text = "This was written by code.\n";
	NxFr::String Line = "This was written by code.";

	TEST(IO, Path)
	{
		NxFr::Path::Info Infos1 = NxFr::Path::Parse("D:/Root/Folder/SubFolder/Name.extension");
		ASSERT_EQ(Infos1.Drive, "D");
		ASSERT_EQ(Infos1.Folder, "Root/Folder/SubFolder");
		ASSERT_EQ(Infos1.Name, "Name");
		ASSERT_EQ(Infos1.Extension, "extension");
		ASSERT_EQ(Infos1.Depth, 3);

		NxFr::Path::Info Infos2 = NxFr::Path::Parse("Root/Folder/SubFolder/Name");
		ASSERT_EQ(Infos2.Drive, "");
		ASSERT_EQ(Infos2.Folder, "Root/Folder/SubFolder");
		ASSERT_EQ(Infos2.Name, "Name");
		ASSERT_EQ(Infos2.Extension, "");
		ASSERT_EQ(Infos2.Depth, 3);

		NxFr::Path::Info Infos3 = NxFr::Path::Parse("/Root/Folder/SubFolder/Name/");
		ASSERT_EQ(Infos3.Drive, "");
		ASSERT_EQ(Infos3.Folder, "Root/Folder/SubFolder");
		ASSERT_EQ(Infos3.Name, "Name");
		ASSERT_EQ(Infos3.Extension, "");
		ASSERT_EQ(Infos3.Depth, 3);

		NxFr::Path::Info Infos4 = NxFr::Path::Parse("/Root/Folder/SubFolder/Name");
		ASSERT_EQ(Infos4.Drive, "");
		ASSERT_EQ(Infos4.Folder, "Root/Folder/SubFolder");
		ASSERT_EQ(Infos4.Name, "Name");
		ASSERT_EQ(Infos4.Extension, "");
		ASSERT_EQ(Infos4.Depth, 3);

		NxFr::Path::Info Infos5 = NxFr::Path::Parse("../Root/Folder/SubFolder/Name");
		ASSERT_EQ(Infos5.Drive, "");
		ASSERT_EQ(Infos5.Folder, "../Root/Folder/SubFolder");
		ASSERT_EQ(Infos5.Name, "Name");
		ASSERT_EQ(Infos5.Extension, "");
		ASSERT_EQ(Infos5.Depth, 3);

		NxFr::Path::Info Infos6 = NxFr::Path::Parse("../Root/Folder/../SubFolder/Name");
		ASSERT_EQ(Infos6.Drive, "");
		ASSERT_EQ(Infos6.Folder, "../Root/Folder/../SubFolder");
		ASSERT_EQ(Infos6.Name, "Name");
		ASSERT_EQ(Infos6.Extension, "");
		ASSERT_EQ(Infos6.Depth, 3);

		NxFr::Path::Info Infos7 = NxFr::Path::Parse("/Name/");
		ASSERT_EQ(Infos7.Drive, "");
		ASSERT_EQ(Infos7.Folder, "");
		ASSERT_EQ(Infos7.Name, "Name");
		ASSERT_EQ(Infos7.Extension, "");
		ASSERT_EQ(Infos7.Depth, 0);


		ASSERT_EQ(NxFr::Path::Exist(NxFr::Path::GetWorkingDirectory()), true);

		ASSERT_EQ(NxFr::Path::IsAbsolute("D:/Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::IsRelative("Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::IsSubdirectory("Root/Folder/SubFolder/", "Root/Folder"), true);
		ASSERT_EQ(NxFr::Path::IsDirectory("D:/Root/Folder/SubFolder/Name/"), true);
		ASSERT_EQ(NxFr::Path::IsFile("D:/Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::HasDrive("D:/Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::HasFolder("D:/Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::HasName("D:/Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::HasExtension("D:/Root/Folder/SubFolder/Name.extension"), true);
		ASSERT_EQ(NxFr::Path::GetDrive("D:/Root/Folder/SubFolder/Name.extension"), "D");
		ASSERT_EQ(NxFr::Path::GetFolder("D:/Root/Folder/SubFolder/Name.extension"), "Root/Folder/SubFolder");
		ASSERT_EQ(NxFr::Path::GetName("D:/Root/Folder/SubFolder/Name.extension"), "Name");
		ASSERT_EQ(NxFr::Path::GetExtension("D:/Root/Folder/SubFolder/Name.extension"), "extension");
		ASSERT_EQ(NxFr::Path::GetDriveAndFolder("D:/Root/Folder/SubFolder/Name.extension"), "D:/Root/Folder/SubFolder");
		ASSERT_EQ(NxFr::Path::GetFolderAndName("D:/Root/Folder/SubFolder/Name.extension"), "Root/Folder/SubFolder/Name");
		ASSERT_EQ(NxFr::Path::GetNameAndExtension("D:/Root/Folder/SubFolder/Name.extension"), "Name.extension");
		ASSERT_EQ(NxFr::Path::GetPathWithoutDrive("D:/Root/Folder/SubFolder/Name.extension"), "Root/Folder/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::GetPathWithoutExtension("D:/Root/Folder/SubFolder/Name.extension"), "D:/Root/Folder/SubFolder/Name");
		ASSERT_EQ(NxFr::Path::Split("D:/Root/Folder/SubFolder/Name.extension").IsEmpty(), false);
		ASSERT_EQ(NxFr::Path::Depth("D:/Root/Folder/SubFolder/Name.extension"), 3);
		ASSERT_EQ(NxFr::Path::ChangeDrive("D:/Root/Folder/SubFolder/Name.extension", "E"), "E:/Root/Folder/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::ChangeFolder("D:/Root/Folder/SubFolder/Name.extension", "Other"), "D:/Other/Name.extension");
		ASSERT_EQ(NxFr::Path::ChangeName("D:/Root/Folder/SubFolder/Name.extension", "Other"), "D:/Root/Folder/SubFolder/Other.extension");
		ASSERT_EQ(NxFr::Path::ChangeExtension("D:/Root/Folder/SubFolder/Name.extension", "other"), "D:/Root/Folder/SubFolder/Name.other");

		ASSERT_EQ(NxFr::Path::Combine("D:", "Root", "Folder/SubFolder", "Name.extension"), "D:/Root/Folder/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::Join("Root", "Folder/SubFolder"), "Root/Folder/SubFolder");
		ASSERT_EQ(NxFr::Path::Normalize("D:\\Root\\Folder//SubFolder\\Name.extension"), "D:/Root/Folder/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::Resolve("D:/Root/Folder/../SubFolder/Name"), "D:/Root/SubFolder/Name");
		ASSERT_EQ(NxFr::Path::Resolve("D:/Root/Folder/../SubFolder/Name/"), "D:/Root/SubFolder/Name/");
		ASSERT_EQ(NxFr::Path::Resolve("D:/Root/Folder/../SubFolder/Name.extension"), "D:/Root/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::Resolve("../Root/Folder/../SubFolder/Name"), "Root/SubFolder/Name");
		ASSERT_EQ(NxFr::Path::MakeRelative("D:/Root/Folder/SubFolder/Name.extension", "D:/Root/Folder/"), "SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::MakeRelative("D:/Root/Other/SubFolder/Name.extension", "D:/Root/Folder"), "../Other/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::MakeAbsolute("SubFolder/Name.extension", "D:/Root/Folder"), "D:/Root/Folder/SubFolder/Name.extension");
		ASSERT_EQ(NxFr::Path::MakeAbsolute("../Other/SubFolder/Name.extension", "D:/Root/Folder"), "D:/Root/Other/SubFolder/Name.extension");
	}

	TEST(IO, Directory)
	{
		NxFr::String Working = NxFr::Path::GetWorkingDirectory();
		NxFr::Directory Directory = NxFr::Directory(Working);

		ASSERT_EQ(Directory.GetPath(), Working);
		ASSERT_EQ(Directory.Exists(), true);
		ASSERT_EQ(Directory.GetCount() > 0, true);

		ASSERT_EQ(Directory.GetContent(true).GetCount() > 0, true);
		ASSERT_EQ(Directory.GetFiles(true).GetCount() > 0, true);
		//ASSERT_EQ(Directory.GetDirectories(true).GetCount() == 0, true);

		NxFr::Directory SubDirectory = NxFr::Directory(NxFr::Path::Combine(Working, "Test"));

		SubDirectory.Create();
		ASSERT_EQ(SubDirectory.Exists(), true);

		NxFr::Directory(NxFr::Path::Combine(Working, "Test", "Test1")).Create();
		NxFr::Directory(NxFr::Path::Combine(Working, "Test", "Test2")).Create();
		NxFr::Directory(NxFr::Path::Combine(Working, "Test", "Test3")).Create();

		SubDirectory.Move(NxFr::Path::Combine(Working, "UnitTest"));
		ASSERT_EQ(SubDirectory.Exists(), true);

		SubDirectory.Delete();
		ASSERT_EQ(SubDirectory.Exists(), false);
	}

	TEST(IO, File)
	{
		NxFr::String Working = NxFr::Path::Combine(NxFr::Path::GetWorkingDirectory(), "UnitTest.txt");
		NxFr::File File = NxFr::File(Working);

		ASSERT_EQ(File.GetPath(), Working);
		ASSERT_EQ(File.Exists(), false);

		File.Create();
		ASSERT_EQ(File.Exists(), true);

		File.Move(NxFr::Path::ChangeName(Working, "UnitTestRenamed"));
		ASSERT_EQ(File.Exists(), true);

		File.Open(NxFr::File::Mode::Write);
		ASSERT_EQ(File.IsOpened(), true);
		File.WriteByte(NxFr::BufferView((NxFr::Byte*)Data, 8 * 10));
		ASSERT_EQ(File.GetSize(), 8 * 10);
		File.Close();
		ASSERT_EQ(File.IsOpened(), false);

		File.Open(NxFr::File::Mode::Read);
		ASSERT_EQ(File.IsOpened(), true);
		NxFr::Buffer Content = File.ReadByte();
		ASSERT_EQ(((uint64*)Content.GetPtr())[5], 15);
		File.Close();
		ASSERT_EQ(File.IsOpened(), false);

		File.Open(NxFr::File::Mode::Append);
		ASSERT_EQ(File.IsOpened(), true);
		File.WriteByte(NxFr::BufferView((NxFr::Byte*)Data, 8 * 10));
		ASSERT_EQ(File.GetSize(), 8 * 10 * 2);
		File.Close();
		ASSERT_EQ(File.IsOpened(), false);

		File.Delete();
		ASSERT_EQ(File.Exists(), false);
	}

	TEST(IO, Text)
	{
		NxFr::String Working = NxFr::Path::Combine(NxFr::Path::GetWorkingDirectory(), "UnitTest.txt");
		NxFr::File File = NxFr::File(Working);

		File.Create();

		File.Open(NxFr::File::Mode::Write);
		File.WriteText(Text);
		ASSERT_EQ(File.GetSize() > 0, true);
		File.Close();

		File.Open(NxFr::File::Mode::Read);
		NxFr::String Content1 = File.ReadText();
		ASSERT_EQ(Content1, Text);
		File.Close();

		File.Open(NxFr::File::Mode::Append);
		File.WriteText(Text);
		File.WriteText(Text);
		File.WriteText(Text);
		ASSERT_EQ(File.GetSize() > 0, true);
		File.Close();

		File.Open(NxFr::File::Mode::Read);
		NxFr::String Content2 = File.ReadText();
		ASSERT_EQ(Content2, Text + Text + Text + Text);
		File.Close();

		File.Delete();
	}

	TEST(IO, TextStream)
	{
		NxFr::String Working = NxFr::Path::Combine(NxFr::Path::GetWorkingDirectory(), "UnitTest.txt");
		NxFr::TextStream File(Working);

		File.Open(NxFr::File::Mode::Write);
		File.WriteLine(Line);
		File.WriteLine(Line);
		File.WriteLine(Line);
		File.Close();

		File.Open(NxFr::File::Mode::Read);
		NxFr::StringView Content1 = File.ReadLine();
		ASSERT_EQ(Content1, Line);
		NxFr::StringView Content2 = File.ReadLine();
		ASSERT_EQ(Content2, Line);
		NxFr::StringView Content3 = File.ReadLine();
		ASSERT_EQ(Content3, Line);
		ASSERT_EQ(File.IsAtTheEnd(), true);
		File.Close();

		NxFr::File(Working).Delete();
	}

	TEST(IO, BinaryStream)
	{
		NxFr::String Working = NxFr::Path::Combine(NxFr::Path::GetWorkingDirectory(), "UnitTest.txt");
		NxFr::BufferView View = NxFr::BufferView(Data, sizeof(Data));
		NxFr::BinaryStream File(Working);

		File.Open(NxFr::File::Mode::Write);
		File.WriteBlock(View);
		File.WriteBlock(View);
		File.WriteBlock(View);
		File.Close();

		File.Open(NxFr::File::Mode::Read);
		NxFr::BufferView Content1 = File.ReadBlock(sizeof(Data));
		ASSERT_EQ(*Content1.GetPtr<uint64>(0), 10);
		ASSERT_EQ(*Content1.GetPtr<uint64>(16), 12);
		ASSERT_EQ(*Content1.GetPtr<uint64>(72), 19);
		NxFr::BufferView Content2 = File.ReadBlock(sizeof(Data));
		ASSERT_EQ(*Content2.GetPtr<uint64>(0), 10);
		ASSERT_EQ(*Content2.GetPtr<uint64>(16), 12);
		ASSERT_EQ(*Content2.GetPtr<uint64>(72), 19);
		NxFr::BufferView Content3 = File.ReadBlock(sizeof(Data));
		ASSERT_EQ(*Content3.GetPtr<uint64>(0), 10);
		ASSERT_EQ(*Content3.GetPtr<uint64>(16), 12);
		ASSERT_EQ(*Content3.GetPtr<uint64>(72), 19);
		File.Close();

		NxFr::File(Working).Delete();
	}
}
