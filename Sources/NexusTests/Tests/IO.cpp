#include "Core/NexusTests.h"

namespace NxTs
{
	uint64 Data[10] = { 10,11,12,13,14,15,16,17,18,19 };
	NxFr::String Text = "This was written by code.\n";

	TEST(IO, Path_Static)
	{
		NxFr::String Working = NxFr::Path::GetWorkingDirectory().ToString();
		NxFr::String WorkingName = Working.SplitAll("/").Last().ToString();
		NxFr::String File = Working + "NexusTests.exe";
		NxFr::String Directory = Working + "Test/Subfolder/Deep/";
		NxFr::String Root = Working + "Test/Other/Deep/";

		NxFr::Array<NxFr::StringView> Items = NxFr::Array<NxFr::StringView>(4);
		Items[0] = "D:";
		Items[1] = "Nexus";
		Items[2] = "Test";
		Items[3] = "Test.txt";

		ASSERT_EQ(NxFr::Path::Exist(Working), true);
		ASSERT_EQ(NxFr::Path::Exist(File), true);
		ASSERT_EQ(NxFr::Path::IsFile(File), true);
		ASSERT_EQ(NxFr::Path::IsFile(Directory), false);
		ASSERT_EQ(NxFr::Path::IsDirectory(Directory), true);
		ASSERT_EQ(NxFr::Path::IsDirectory(File), false);
		ASSERT_EQ(NxFr::Path::IsAbsolute(File), true);
		ASSERT_EQ(NxFr::Path::IsRelative(File), false);
		ASSERT_EQ(NxFr::Path::HasExtension(File, ".exe"), true);
		ASSERT_EQ(NxFr::Path::GetDrive(File), "D");
		ASSERT_EQ(NxFr::Path::GetDirectoryPath(File), Working);
		ASSERT_EQ(NxFr::Path::GetDirectoryName(File), WorkingName);
		ASSERT_EQ(NxFr::Path::GetParent(File), Working);
		ASSERT_EQ(NxFr::Path::GetParent(Directory), Working + "Test/Subfolder/");
		ASSERT_EQ(NxFr::Path::GetFileName(File), "NexusTests");
		ASSERT_EQ(NxFr::Path::GetFileName(File, true), "NexusTests.exe");
		ASSERT_EQ(NxFr::Path::GetExtension(File), "exe");

		ASSERT_EQ(NxFr::Path::ChangeFileName(File.ToView(), "Modified.exe"), Working + "Modified.exe");
		ASSERT_EQ(NxFr::Path::ChangeDirectoryPath(File.ToView(), "D:/Test/"), "D:/Test/NexusTests.exe");
		ASSERT_EQ(NxFr::Path::ChangeExtension(File.ToView(), "txt"), Working + "NexusTests.txt");
		ASSERT_EQ(NxFr::Path::ConvertAbsoluteToRelative(Directory.ToView(), Root), "../../Subfolder/Deep/");
		ASSERT_EQ(NxFr::Path::ConvertRelativeToAbsolute("../../Subfolder/Deep/", Root), Directory);
		ASSERT_EQ(NxFr::Path::Resolve("D:/Nexus/Test/../../Test/Subfolder/Deep/"), "D:/Test/Subfolder/Deep/");
		ASSERT_EQ(NxFr::Path::Resolve(Working + "Test/Subfolder/Deep/"), Directory);
		ASSERT_EQ(NxFr::Path::Normalize("Test//Subfolder\\Deep"), "Test/Subfolder/Deep/");

		ASSERT_EQ(NxFr::Path::Combine("D:/Nexus/Test", "Test"), "D:/Nexus/Test/Test/");
		ASSERT_EQ(NxFr::Path::Combine("D:/Nexus/Test", "Test.txt"), "D:/Nexus/Test/Test.txt");
		ASSERT_EQ(NxFr::Path::Combine<NxFr::StringView>(Items), "D:/Nexus/Test/Test.txt");
		ASSERT_EQ(NxFr::Path::Previous(Directory.ToView(), 2), Working + "Test/");
	}

	TEST(IO, Path_Instance)
	{
		NxFr::Path Working = NxFr::Path::GetWorkingDirectory();
		NxFr::Path WorkingName = Working.Split().Last();
		NxFr::Path File = Working + "NexusTests.exe";
		NxFr::Path Directory = Working + "Test/Subfolder/Deep/";
		NxFr::Path Root = Working + "Test/Other/Deep/";
		NxFr::Path Relative = NxFr::Path("../../Subfolder/Deep/");
		NxFr::Path ToResolve1 = NxFr::Path("D:/Nexus/Test/../../Test/Subfolder/Deep/");
		NxFr::Path ToResolve2 = Working + "Test/Subfolder/Deep/";
		NxFr::Path ToNormalize = NxFr::Path("Test//Subfolder\\Deep");

		NxFr::Array<NxFr::StringView> Items = NxFr::Array<NxFr::StringView>(3);
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
		ASSERT_EQ(File.GetFileName(), "NexusTests");
		ASSERT_EQ(File.GetFileName(true), "NexusTests.exe");
		ASSERT_EQ(File.GetExtension(), "exe");

		ASSERT_EQ(File.ChangeDirectoryPath("D:/Test/"), "D:/Test/NexusTests.exe");
		ASSERT_EQ(File.ChangeFileName("Modified.exe"), "D:/Test/Modified.exe");
		ASSERT_EQ(File.ChangeExtension("txt"), "D:/Test/Modified.txt");
		ASSERT_EQ(Directory.ConvertAbsoluteToRelative(Root), "../../Subfolder/Deep/");
		ASSERT_EQ(Relative.ConvertRelativeToAbsolute(Root), Working + "Test/Subfolder/Deep/");
		ASSERT_EQ(ToResolve1.Resolve(), "D:/Test/Subfolder/Deep/");
		ASSERT_EQ(ToResolve2.Resolve(), Working + "Test/Subfolder/Deep/");
		ASSERT_EQ(ToNormalize.Normalize(), "Test/Subfolder/Deep/");

		ASSERT_EQ(Directory.Join("Test"), "../../Subfolder/Deep/Test/");
		ASSERT_EQ(Directory.Join("Test.txt"), "../../Subfolder/Deep/Test/Test.txt");
		ASSERT_EQ(Root.Join<NxFr::StringView>(Items), Working + "Test/Other/Deep/" + "Nexus/Test/Test.txt");
		ASSERT_EQ(Directory.Previous(2), "../../Subfolder/Deep/");
	}

	TEST(IO, Directory)
	{
		NxFr::Path Working = NxFr::Path::GetWorkingDirectory();
		NxFr::Directory Directory = NxFr::Directory(Working);

		ASSERT_EQ(Directory.GetPath(), Working);
		ASSERT_EQ(Directory.Exists(), true);
		ASSERT_EQ(Directory.GetCount() > 0, true);

		ASSERT_EQ(Directory.GetContent(true).GetCount() > 0, true);
		ASSERT_EQ(Directory.GetFiles(true).GetCount() > 0, true);
		ASSERT_EQ(Directory.GetDirectories(true).GetCount() == 0, true);

		NxFr::Directory SubDirectory = NxFr::Directory(Working + "Test");

		SubDirectory.Create();
		ASSERT_EQ(SubDirectory.Exists(), true);

		NxFr::Directory(Working + "Test" + "Test1").Create();
		NxFr::Directory(Working + "Test" + "Test2").Create();
		NxFr::Directory(Working + "Test" + "Test3").Create();

		SubDirectory.Move(Working + "UnitTest");
		ASSERT_EQ(SubDirectory.Exists(), true);

		SubDirectory.Delete();
		ASSERT_EQ(SubDirectory.Exists(), false);
	}

	TEST(IO, File)
	{
		NxFr::Path Working = NxFr::Path::GetWorkingDirectory() + "UnitTest.txt";
		NxFr::File File = NxFr::File(Working);

		ASSERT_EQ(File.GetPath(), Working);
		ASSERT_EQ(File.Exists(), false);

		File.Create();
		ASSERT_EQ(File.Exists(), true);

		File.Move(Working.ChangeFileName("UnitTestRenamed.txt"));
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
		NxFr::Path Working = NxFr::Path::GetWorkingDirectory() + "UnitTestText.txt";
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
}
