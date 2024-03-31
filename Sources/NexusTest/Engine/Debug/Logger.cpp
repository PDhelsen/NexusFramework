#include "Core/NexusTestPch.h"

#include "Debug/Logger.h"

namespace NxTs
{
	TEST(Logger, Channel)
	{
		NxEn::Logger* Logger = NxEn::Logger::GetInstance();

		Logger->AddChannel(1, false);
		ASSERT_EQ(Logger->HasChannel(1), true);
		ASSERT_EQ(Logger->CheckChannel(1), false);

		Logger->SetChannel(1, true);
		ASSERT_EQ(Logger->CheckChannel(1), true);
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
