#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	uint64 Data[10] = { 10,11,12,13,14,15,16,17,18,19 };
	NxFr::String Text = "This was written by code.\n";

	NxFr::Dictionary<NxFr::String, const NxFr::String> CreateTestsFiles()
	{
		NxFr::String Root = NxFr::Path::GetWorkingDirectory().ToString();
		NxFr::String UnitTestDirectory = Root + "UnitTest/";
		NxFr::String UnitTestSubDirectory = UnitTestDirectory + "SubDir/";
		NxFr::String UnitTestFile = UnitTestDirectory + "UnitTest.txt";
		NxFr::String UnitTestSubFile = UnitTestSubDirectory + "UnitTest.txt";

		NxFr::Directory(UnitTestDirectory).Create();
		NxFr::Directory(UnitTestSubDirectory).Create();
		NxFr::File(UnitTestFile).Create();
		NxFr::File(UnitTestSubFile).Create();

		NxFr::Dictionary<NxFr::String, const NxFr::String> TestsFiles;
		TestsFiles.Append("UnitTestDirectory", UnitTestDirectory);
		TestsFiles.Append("UnitTestSubDirectory", UnitTestSubDirectory);
		TestsFiles.Append("UnitTestFile", UnitTestFile);
		TestsFiles.Append("UnitTestSubFile", UnitTestSubFile);
		TestsFiles.Append("UnitTestDirectoryName", "UnitTest");
		TestsFiles.Append("UnitTestFileName", "UnitTest.txt");
		return TestsFiles;
	}

	void DeleteTestsFiles(const NxFr::Dictionary<NxFr::String, const NxFr::String>& TestsFiles)
	{
		NxFr::Directory(TestsFiles["UnitTestDirectory"]).Delete();
	}

	TEST(IO, Path_Static)
	{
		NxFr::Dictionary<NxFr::String, const NxFr::String> TestsFiles = CreateTestsFiles();

		NxFr::Array<NxFr::StringView> Items = NxFr::Array<NxFr::StringView>(2);
		Items[0] = TestsFiles["UnitTestDirectory"];
		Items[1] = TestsFiles["UnitTestFileName"];

		ASSERT_EQ(NxFr::Path::Exist(TestsFiles["UnitTestDirectory"]), true);
		ASSERT_EQ(NxFr::Path::Exist(TestsFiles["UnitTestFile"]), true);
		ASSERT_EQ(NxFr::Path::IsFile(TestsFiles["UnitTestFile"]), true);
		ASSERT_EQ(NxFr::Path::IsFile(TestsFiles["UnitTestDirectory"]), false);
		ASSERT_EQ(NxFr::Path::IsDirectory(TestsFiles["UnitTestDirectory"]), true);
		ASSERT_EQ(NxFr::Path::IsDirectory(TestsFiles["UnitTestFile"]), false);
		ASSERT_EQ(NxFr::Path::IsAbsolute(TestsFiles["UnitTestFile"]), true);
		ASSERT_EQ(NxFr::Path::IsRelative(TestsFiles["UnitTestFile"]), false);
		ASSERT_EQ(NxFr::Path::HasExtension(TestsFiles["UnitTestFile"], ".txt"), true);
		ASSERT_EQ(NxFr::Path::GetDrive(TestsFiles["UnitTestFile"]), "D");
		ASSERT_EQ(NxFr::Path::GetDirectoryPath(TestsFiles["UnitTestFile"]), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(NxFr::Path::GetDirectoryName(TestsFiles["UnitTestFile"]), TestsFiles["UnitTestDirectoryName"]);
		ASSERT_EQ(NxFr::Path::GetParent(TestsFiles["UnitTestFile"]), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(NxFr::Path::GetParent(TestsFiles["UnitTestSubDirectory"]), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(NxFr::Path::GetFileName(TestsFiles["UnitTestFile"]), "UnitTest");
		ASSERT_EQ(NxFr::Path::GetFileName(TestsFiles["UnitTestFile"], true), TestsFiles["UnitTestFileName"]);
		ASSERT_EQ(NxFr::Path::GetExtension(TestsFiles["UnitTestFile"]), "txt");

		ASSERT_EQ(NxFr::Path::ChangeFileName(TestsFiles["UnitTestFile"], "Modified.txt"), TestsFiles["UnitTestDirectory"] + "Modified.txt");
		ASSERT_EQ(NxFr::Path::ChangeDirectoryPath(TestsFiles["UnitTestSubFile"], TestsFiles["UnitTestDirectory"]), TestsFiles["UnitTestFile"]);
		ASSERT_EQ(NxFr::Path::ChangeExtension(TestsFiles["UnitTestFile"], "exe"), TestsFiles["UnitTestDirectory"] + "UnitTest.exe");
		ASSERT_EQ(NxFr::Path::ConvertAbsoluteToRelative(TestsFiles["UnitTestSubDirectory"], TestsFiles["UnitTestDirectory"]), "SubDir/");
		ASSERT_EQ(NxFr::Path::ConvertRelativeToAbsolute("SubDir/", TestsFiles["UnitTestDirectory"]), TestsFiles["UnitTestSubDirectory"]);
		ASSERT_EQ(NxFr::Path::Resolve(TestsFiles["UnitTestSubDirectory"] + "../"), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(NxFr::Path::Normalize(TestsFiles["UnitTestDirectory"] + "\\Test\\"), TestsFiles["UnitTestDirectory"] + "Test/");

		ASSERT_EQ(NxFr::Path::Combine(TestsFiles["UnitTestDirectory"], TestsFiles["UnitTestFileName"]), TestsFiles["UnitTestFile"]);
		ASSERT_EQ(NxFr::Path::Combine<NxFr::StringView>(Items), TestsFiles["UnitTestFile"]);
		ASSERT_EQ(NxFr::Path::Previous(TestsFiles["UnitTestSubDirectory"], 1), TestsFiles["UnitTestDirectory"]);

		DeleteTestsFiles(TestsFiles);
	}

	TEST(IO, Path_Instance)
	{
		NxFr::Dictionary<NxFr::String, const NxFr::String> TestsFiles = CreateTestsFiles();

		NxFr::Path File = NxFr::Path(TestsFiles["UnitTestFile"]);
		NxFr::Path Directory = NxFr::Path(TestsFiles["UnitTestSubDirectory"]);

		ASSERT_EQ(Directory.Exist(), true);
		ASSERT_EQ(File.Exist(), true);
		ASSERT_EQ(File.IsFile(), true);
		ASSERT_EQ(Directory.IsFile(), false);
		ASSERT_EQ(Directory.IsDirectory(), true);
		ASSERT_EQ(File.IsDirectory(), false);
		ASSERT_EQ(File.IsAbsolute(), true);
		ASSERT_EQ(File.IsRelative(), false);
		ASSERT_EQ(File.HasExtension(".txt"), true);
		ASSERT_EQ(File.GetDrive(), "D");
		ASSERT_EQ(File.GetDirectoryPath(), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(File.GetDirectoryName(), TestsFiles["UnitTestDirectoryName"]);
		ASSERT_EQ(File.GetParent(), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(Directory.GetParent(), TestsFiles["UnitTestDirectory"]);
		ASSERT_EQ(File.GetFileName(true), TestsFiles["UnitTestFileName"]);
		ASSERT_EQ(File.GetExtension(), "txt");

		ASSERT_EQ(File.ChangeDirectoryPath(TestsFiles["UnitTestSubDirectory"]), TestsFiles["UnitTestSubFile"]);
		ASSERT_EQ(File.ChangeFileName("Modified.txt"), TestsFiles["UnitTestSubDirectory"] + "Modified.txt");
		ASSERT_EQ(File.ChangeExtension("exe"), TestsFiles["UnitTestSubDirectory"] + "Modified.exe");
		ASSERT_EQ(Directory.ConvertAbsoluteToRelative(TestsFiles["UnitTestDirectory"]), "SubDir/");
		ASSERT_EQ(Directory.ConvertRelativeToAbsolute(TestsFiles["UnitTestDirectory"]), TestsFiles["UnitTestSubDirectory"]);

		ASSERT_EQ(Directory.Join("UnitTest.txt"), TestsFiles["UnitTestSubFile"]);
		ASSERT_EQ(Directory.Previous(1), TestsFiles["UnitTestSubDirectory"]);

		DeleteTestsFiles(TestsFiles);
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
		//ASSERT_EQ(Directory.GetDirectories(true).GetCount() == 0, true);

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
