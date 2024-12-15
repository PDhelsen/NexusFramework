#include "Core/NexusTestPch.h"

#include "Misc/IO/Path.h"
#include "Misc/IO/Directory.h"
#include "Misc/IO/File.h"

namespace NxTs
{
	uint64 Data[10] = { 10,11,12,13,14,15,16,17,18,19 };
	NxEn::String Text = "This was written by code.\n";

	TEST(IO, Path_Static)
	{
		NxEn::String Working = NxEn::Path::GetWorkingDirectory().ToString();
		NxEn::String WorkingName = Working.SplitAll("/").Last().ToString();
		NxEn::String File = Working + "NexusTest.exe";
		NxEn::String Directory = Working + "Test/Subfolder/Deep/";
		NxEn::String Root = Working + "Test/Other/Deep/";

		NxEn::Array<NxEn::StringView> Items = NxEn::Array<NxEn::StringView>(4);
		Items[0] = "D:";
		Items[1] = "Nexus";
		Items[2] = "Test";
		Items[3] = "Test.txt";

		ASSERT_EQ(NxEn::Path::Exist(Working), true);
		ASSERT_EQ(NxEn::Path::Exist(File), true);
		ASSERT_EQ(NxEn::Path::IsFile(File), true);
		ASSERT_EQ(NxEn::Path::IsFile(Directory), false);
		ASSERT_EQ(NxEn::Path::IsDirectory(Directory), true);
		ASSERT_EQ(NxEn::Path::IsDirectory(File), false);
		ASSERT_EQ(NxEn::Path::IsAbsolute(File), true);
		ASSERT_EQ(NxEn::Path::IsRelative(File), false);
		ASSERT_EQ(NxEn::Path::HasExtension(File, ".exe"), true);
		ASSERT_EQ(NxEn::Path::GetDrive(File), "D");
		ASSERT_EQ(NxEn::Path::GetDirectoryPath(File), Working);
		ASSERT_EQ(NxEn::Path::GetDirectoryName(File), WorkingName);
		ASSERT_EQ(NxEn::Path::GetParent(File), Working);
		ASSERT_EQ(NxEn::Path::GetParent(Directory), Working + "Test/Subfolder/");
		ASSERT_EQ(NxEn::Path::GetFileName(File), "NexusTest");
		ASSERT_EQ(NxEn::Path::GetFileName(File, true), "NexusTest.exe");
		ASSERT_EQ(NxEn::Path::GetExtension(File), "exe");

		ASSERT_EQ(NxEn::Path::ChangeFileName(File.ToView(), "Modified.exe"), Working + "Modified.exe");
		ASSERT_EQ(NxEn::Path::ChangeDirectoryPath(File.ToView(), "D:/Test/"), "D:/Test/NexusTest.exe");
		ASSERT_EQ(NxEn::Path::ChangeExtension(File.ToView(), "txt"), Working + "NexusTest.txt");
		ASSERT_EQ(NxEn::Path::ConvertAbsoluteToRelative(Directory.ToView(), Root), "../../Subfolder/Deep/");
		ASSERT_EQ(NxEn::Path::ConvertRelativeToAbsolute("../../Subfolder/Deep/", Root), Directory);
		ASSERT_EQ(NxEn::Path::Resolve("D:/Nexus/Test/../../Test/Subfolder/Deep/"), "D:/Test/Subfolder/Deep/");
		ASSERT_EQ(NxEn::Path::Resolve(Working + "Test/Subfolder/Deep/"), Directory);
		ASSERT_EQ(NxEn::Path::Normalize("Test//Subfolder\\Deep"), "Test/Subfolder/Deep/");

		ASSERT_EQ(NxEn::Path::Combine("D:/Nexus/Test", "Test"), "D:/Nexus/Test/Test/");
		ASSERT_EQ(NxEn::Path::Combine("D:/Nexus/Test", "Test.txt"), "D:/Nexus/Test/Test.txt");
		ASSERT_EQ(NxEn::Path::Combine<NxEn::StringView>(Items), "D:/Nexus/Test/Test.txt");
		ASSERT_EQ(NxEn::Path::Previous(Directory.ToView(), 2), Working + "Test/");
	}

	TEST(IO, Path_Instance)
	{
		NxEn::Path Working = NxEn::Path::GetWorkingDirectory();
		NxEn::Path WorkingName = Working.Split().Last();
		NxEn::Path File = Working + "NexusTest.exe";
		NxEn::Path Directory = Working + "Test/Subfolder/Deep/";
		NxEn::Path Root = Working + "Test/Other/Deep/";
		NxEn::Path Relative = NxEn::Path("../../Subfolder/Deep/");
		NxEn::Path ToResolve1 = NxEn::Path("D:/Nexus/Test/../../Test/Subfolder/Deep/");
		NxEn::Path ToResolve2 = Working + "Test/Subfolder/Deep/";
		NxEn::Path ToNormalize = NxEn::Path("Test//Subfolder\\Deep");

		NxEn::Array<NxEn::StringView> Items = NxEn::Array<NxEn::StringView>(3);
		Items[0] = "Nexus";
		Items[1] = "Test";
		Items[2] = "Test.txt";

		ASSERT_EQ(Working.Exist(), true);
		ASSERT_EQ(File.Exist(), true);
		ASSERT_EQ(File.IsFile(), true);
		ASSERT_EQ(Directory.IsFile(), false);
		ASSERT_EQ(Directory.IsDirectory(), true);
		ASSERT_EQ(File.IsDirectory(), false);
		ASSERT_EQ(File.IsAbsolute(), true);
		ASSERT_EQ(File.IsRelative(), false);
		ASSERT_EQ(File.HasExtension(".exe"), true);
		ASSERT_EQ(File.GetDrive(), "D");
		ASSERT_EQ(File.GetDirectoryPath(), Working);
		ASSERT_EQ(File.GetDirectoryName(), WorkingName);
		ASSERT_EQ(File.GetParent(), Working);
		ASSERT_EQ(Directory.GetParent(), Working + "Test/Subfolder/");
		ASSERT_EQ(File.GetFileName(), "NexusTest");
		ASSERT_EQ(File.GetFileName(true), "NexusTest.exe");
		ASSERT_EQ(File.GetExtension(), "exe");

		ASSERT_EQ(File.ChangeDirectoryPath("D:/Test/"), "D:/Test/NexusTest.exe");
		ASSERT_EQ(File.ChangeFileName("Modified.exe"), "D:/Test/Modified.exe");
		ASSERT_EQ(File.ChangeExtension("txt"), "D:/Test/Modified.txt");
		ASSERT_EQ(Directory.ConvertAbsoluteToRelative(Root), "../../Subfolder/Deep/");
		ASSERT_EQ(Relative.ConvertRelativeToAbsolute(Root), Working + "Test/Subfolder/Deep/");
		ASSERT_EQ(ToResolve1.Resolve(), "D:/Test/Subfolder/Deep/");
		ASSERT_EQ(ToResolve2.Resolve(), Working + "Test/Subfolder/Deep/");
		ASSERT_EQ(ToNormalize.Normalize(), "Test/Subfolder/Deep/");

		ASSERT_EQ(Directory.Join("Test"), "../../Subfolder/Deep/Test/");
		ASSERT_EQ(Directory.Join("Test.txt"), "../../Subfolder/Deep/Test/Test.txt");
		ASSERT_EQ(Root.Join<NxEn::StringView>(Items), Working + "Test/Other/Deep/" + "Nexus/Test/Test.txt");
		ASSERT_EQ(Directory.Previous(2), "../../Subfolder/Deep/");
	}

	TEST(IO, Directory)
	{
		NxEn::Path Working = NxEn::Path::GetWorkingDirectory();
		NxEn::Directory Directory = NxEn::Directory(Working);

		ASSERT_EQ(Directory.GetPath(), Working);
		ASSERT_EQ(Directory.Exists(), true);
		ASSERT_EQ(Directory.GetCount() > 0, true);

		ASSERT_EQ(Directory.GetContent(true).GetCount() > 0, true);
		ASSERT_EQ(Directory.GetFiles(true).GetCount() > 0, true);
		ASSERT_EQ(Directory.GetDirectories(true).GetCount() == 0, true);

		NxEn::Directory SubDirectory = NxEn::Directory(Working + "Test");

		SubDirectory.Create();
		ASSERT_EQ(SubDirectory.Exists(), true);

		NxEn::Directory(Working + "Test" + "Test1").Create();
		NxEn::Directory(Working + "Test" + "Test2").Create();
		NxEn::Directory(Working + "Test" + "Test3").Create();

		SubDirectory.Move(Working + "UnitTest");
		ASSERT_EQ(SubDirectory.Exists(), true);

		SubDirectory.Delete();
		ASSERT_EQ(SubDirectory.Exists(), false);
	}

	TEST(IO, File)
	{
		NxEn::Path Working = NxEn::Path::GetWorkingDirectory() + "UnitTest.txt";
		NxEn::File File = NxEn::File(Working);

		ASSERT_EQ(File.GetPath(), Working);
		ASSERT_EQ(File.Exists(), false);

		File.Create();
		ASSERT_EQ(File.Exists(), true);

		File.Move(Working.ChangeFileName("UnitTestRenamed.txt"));
		ASSERT_EQ(File.Exists(), true);

		File.Open(NxEn::File::Mode::Write);
		ASSERT_EQ(File.IsOpened(), true);
		File.WriteByte(NxEn::BufferView((NxEn::Byte*)Data, 8 * 10));
		ASSERT_EQ(File.GetSize(), 8 * 10);
		File.Close();
		ASSERT_EQ(File.IsOpened(), false);

		File.Open(NxEn::File::Mode::Read);
		ASSERT_EQ(File.IsOpened(), true);
		NxEn::Buffer Content = File.ReadByte();
		ASSERT_EQ(((uint64*)Content.GetPtr())[5], 15);
		File.Close();
		ASSERT_EQ(File.IsOpened(), false);

		File.Open(NxEn::File::Mode::Append);
		ASSERT_EQ(File.IsOpened(), true);
		File.WriteByte(NxEn::BufferView((NxEn::Byte*)Data, 8 * 10));
		ASSERT_EQ(File.GetSize(), 8 * 10 * 2);
		File.Close();
		ASSERT_EQ(File.IsOpened(), false);

		File.Delete();
		ASSERT_EQ(File.Exists(), false);
	}

	TEST(IO, Text)
	{
		NxEn::Path Working = NxEn::Path::GetWorkingDirectory() + "UnitTestText.txt";
		NxEn::File File = NxEn::File(Working);

		File.Create();

		File.Open(NxEn::File::Mode::Write);
		File.WriteText(Text);
		ASSERT_EQ(File.GetSize() > 0, true);
		File.Close();

		File.Open(NxEn::File::Mode::Read);
		NxEn::String Content1 = File.ReadText();
		ASSERT_EQ(Content1, Text);
		File.Close();

		File.Open(NxEn::File::Mode::Append);
		File.WriteText(Text);
		ASSERT_EQ(File.GetSize() > 0, true);
		File.Close();

		File.Open(NxEn::File::Mode::Read);
		NxEn::String Content2 = File.ReadText();
		ASSERT_EQ(Content2, Text + Text);
		File.Close();

		File.Delete();
	}
}
