#include "Core/NexusTestPch.h"

#include "Debug/Logger.h"

namespace NxTs
{
	NxEn::StringId ChannelTest = "Test"_Sid;

	TEST(Logger, Channel)
	{
		NxEn::Logger* Logger = NxEn::Logger::GetInstance();

		Logger->AddChannel(ChannelTest, false);
		ASSERT_EQ(Logger->HasChannel(ChannelTest), true);
		ASSERT_EQ(Logger->CheckChannel(ChannelTest), false);

		Logger->SetChannel(ChannelTest, true);
		ASSERT_EQ(Logger->CheckChannel(ChannelTest), true);
	}

	TEST(Logger, Verbosity)
	{
		NxEn::Logger* Logger = NxEn::Logger::GetInstance();

		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::All), true);
		Logger->SetVerbosity(NxEn::LoggerVerbosity::All, false);

		Logger->SetVerbosity(NxEn::LoggerVerbosity::Warning, true);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Info), false);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Warning), true);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Error), false);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Fatal), false);
		Logger->SetVerbosity(NxEn::LoggerVerbosity::All, false);

		Logger->SetVerbosity(NxEn::LoggerVerbosity::Warning | NxEn::LoggerVerbosity::Fatal, true);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Info), false);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Warning), true);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Error), false);
		ASSERT_EQ(Logger->CheckVerbosity(NxEn::LoggerVerbosity::Fatal), true);
		Logger->SetVerbosity(NxEn::LoggerVerbosity::All, false);

		Logger->SetVerbosity(NxEn::LoggerVerbosity::All, true);
	}
}
