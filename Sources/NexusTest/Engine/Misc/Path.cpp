#include "Core/NexusTestPch.h"

#include "Misc/IO/Path.h"

namespace NxTs
{
	TEST(IO, Path)
	{
		NxEn::String Working = NxEn::Path::GetWorkingDirectory();
		NxEn::StringView WorkingName = Working.SplitAll("/").Last();
		NxEn::String File = Working + "NexusTest.exe";
		NxEn::String Directory = Working + "Test/Subfolder/Deep/";
		NxEn::String Root = Working + "Test/Other/Deep/";

		NxEn::Array<NxEn::StringView> Items = NxEn::Array<NxEn::StringView>(4);
		Items[0] = "D:";
		Items[1] = "Nexus";
		Items[2] = "Test";
		Items[3] = "Test.txt";

		NEXUS_LOG(App, Info, NxEn::LoggerChannel::Default, "Working Directory %s", Working.C());

		ASSERT_EQ(NxEn::Path::Exist(Working), true);
		ASSERT_EQ(NxEn::Path::Exist(File), true);
		ASSERT_EQ(NxEn::Path::IsFile(File), true);
		ASSERT_EQ(NxEn::Path::IsFile(Directory), false);
		ASSERT_EQ(NxEn::Path::IsDirectory(Directory), true);
		ASSERT_EQ(NxEn::Path::IsDirectory(File), false);
		ASSERT_EQ(NxEn::Path::IsAbsolute(File), true);
		ASSERT_EQ(NxEn::Path::IsRelative(File), false);
		ASSERT_EQ(NxEn::Path::HasExtension(File, ".exe"), true);
		ASSERT_EQ(NxEn::Path::GetFileName(File), "NexusTest");
		ASSERT_EQ(NxEn::Path::GetFileName(File, true), "NexusTest.exe");
		ASSERT_EQ(NxEn::Path::GetDirectoryPath(File), Working);
		ASSERT_EQ(NxEn::Path::GetDirectoryName(File), WorkingName);
		ASSERT_EQ(NxEn::Path::GetExtension(File), "exe");
		ASSERT_EQ(NxEn::Path::GetDrive(File), "D");

		ASSERT_EQ(NxEn::Path::Combine("D:/Nexus/Test", "Test.txt"), "D:/Nexus/Test/Test.txt");
		ASSERT_EQ(NxEn::Path::Combine("D:/Nexus/Test", "Test", true), "D:/Nexus/Test/Test/");
		ASSERT_EQ(NxEn::Path::Combine<NxEn::StringView>(Items), "D:/Nexus/Test/Test.txt");
		ASSERT_EQ(NxEn::Path::Combine<NxEn::StringView>(Items, true), "D:/Nexus/Test/Test.txt/");
		ASSERT_EQ(NxEn::Path::ChangeFileName(File, "Modified.exe"), Working + "Modified.exe");
		ASSERT_EQ(NxEn::Path::ChangeDirectoryPath(File, "D:/Test/"), "D:/Test/NexusTest.exe");
		ASSERT_EQ(NxEn::Path::ChangeExtension(File, "txt"), Working + "NexusTest.txt");
		ASSERT_EQ(NxEn::Path::ConvertAbsoluteToRelative(Directory, Root), "../../Subfolder/Deep/");
		ASSERT_EQ(NxEn::Path::ConvertRelativeToAbsolute("../../Subfolder/Deep/", Root), Directory);
		ASSERT_EQ(NxEn::Path::Resolve("D:/Nexus/Test/../../Test/Subfolder/Deep/"), "D:/Test/Subfolder/Deep/");
		ASSERT_EQ(NxEn::Path::Resolve(Working + "Test/Subfolder/Deep/"), Directory);
		ASSERT_EQ(NxEn::Path::Normalize("Test//Subfolder\\Deep", true), "Test/Subfolder/Deep/");
	}
}
