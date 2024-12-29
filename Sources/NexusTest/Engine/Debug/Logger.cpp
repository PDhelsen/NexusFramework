#include "Core/NexusTestPch.h"

#include "Debug/Logger.h"
#include "Misc/IO/Path.h"

namespace NxTs
{
	NxEn::StringId ChannelTest = "Test"_Sid;

	TEST(Logger, File)
	{
		NxEn::Logger Logger = NxEn::Logger(false, NxEn::LoggerVerbosity::All, NxEn::LoggerOutput::All, NxEn::Path::GetWorkingDirectory() + "Logs.txt");
		Logger.AddChannel(ChannelTest, true);
		Logger.Log(NxEn::LoggerVerbosity::Info, ChannelTest, "Test");
		Logger.Log(NxEn::LoggerVerbosity::Warning, ChannelTest, "Test: %d", 10);
		Logger.Log(NxEn::LoggerVerbosity::Error, ChannelTest, "Test: %f", 10.0f);
		Logger.Log(NxEn::LoggerVerbosity::Fatal, ChannelTest, "Test: %s", "Hello World");
	}

	TEST(Logger, Channel)
	{
		NxEn::Logger Logger = NxEn::Logger(false, NxEn::LoggerVerbosity::All, NxEn::LoggerOutput::Console);

		Logger.AddChannel(ChannelTest, false);
		ASSERT_EQ(Logger.HasChannel(ChannelTest), true);
		ASSERT_EQ(Logger.CheckChannel(ChannelTest), false);

		Logger.SetChannel(ChannelTest, true);
		ASSERT_EQ(Logger.CheckChannel(ChannelTest), true);
	}

	TEST(Logger, Verbosity)
	{
		NxEn::Logger Logger = NxEn::Logger(false, NxEn::LoggerVerbosity::All, NxEn::LoggerOutput::Console);

		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::All), true);
		Logger.SetVerbosity(NxEn::LoggerVerbosity::All, false);

		Logger.SetVerbosity(NxEn::LoggerVerbosity::Warning, true);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Info), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Warning), true);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Error), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Fatal), false);
		Logger.SetVerbosity(NxEn::LoggerVerbosity::All, false);

		Logger.SetVerbosity(NxEn::LoggerVerbosity::Warning | NxEn::LoggerVerbosity::Fatal, true);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Info), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Warning), true);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Error), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxEn::LoggerVerbosity::Fatal), true);
		Logger.SetVerbosity(NxEn::LoggerVerbosity::All, false);

		Logger.SetVerbosity(NxEn::LoggerVerbosity::All, true);
	}
}
