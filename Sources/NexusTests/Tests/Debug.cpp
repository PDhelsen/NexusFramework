#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

#include "NexusFramework/Debug/Profiler/Instruments/ChromeTracing.h"

namespace NxTs
{
	TEST(Debug, Logger)
	{
		NxFr::String Path = Dummy::GeneratePath("Debug_Logs.txt");
		NxFr::StringId ChannelId = "Channel"_Sid;

		NxFr::Logger Logger = NxFr::Logger(NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, Path, true);

		Logger.AddChannel(ChannelId, false);
		ASSERT_EQ(Logger.HasChannel(ChannelId), true);
		ASSERT_EQ(Logger.CheckChannel(ChannelId), false);
		Logger.SetChannel(ChannelId, true);
		ASSERT_EQ(Logger.CheckChannel(ChannelId), true);

		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::All), true);
		Logger.SetVerbosity(NxFr::LoggerVerbosity::All, false);
		Logger.SetVerbosity(NxFr::LoggerVerbosity::Warning, true);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Info), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Warning), true);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Error), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Fatal), false);
		Logger.SetVerbosity(NxFr::LoggerVerbosity::All, false);
		Logger.SetVerbosity(NxFr::LoggerVerbosity::Warning | NxFr::LoggerVerbosity::Fatal, true);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Info), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Warning), true);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Error), false);
		ASSERT_EQ(Logger.CheckVerbosity(NxFr::LoggerVerbosity::Fatal), true);
		Logger.SetVerbosity(NxFr::LoggerVerbosity::All, false);
		Logger.SetVerbosity(NxFr::LoggerVerbosity::All, true);

		Logger.LogMessage(NxFr::LoggerVerbosity::Info,		ChannelId, "Test");
		Logger.LogMessage(NxFr::LoggerVerbosity::Warning,	ChannelId, "Test: %d", 10);
		Logger.LogMessage(NxFr::LoggerVerbosity::Error,		ChannelId, "Test: %f", 10.0f);
		Logger.LogMessage(NxFr::LoggerVerbosity::Fatal,		ChannelId, "Test: %s", "Hello World");
	}

	TEST(Debug, Stats)
	{
		NxFr::String Path = Dummy::GeneratePath("Debug_Stats.csv");
		NxFr::StringId LabelId = "Label"_Sid;
		NxFr::StringId CheckId = "Check"_Sid;
		NxFr::StringId SetId = "Set"_Sid;
		NxFr::StringId MinId = "Min"_Sid;
		NxFr::StringId MaxId = "Max"_Sid;
		NxFr::StringId AvgId = "Avg"_Sid;
		NxFr::StringId CntId = "Cnt"_Sid;
		NxFr::StringId AddId = "Add"_Sid;

		NxFr::Stats Stats(Path);

		Stats.RecordHeader(LabelId, NxFr::Stats::StatType::Label, NxFr::Stats::StatMode::Set);
		Stats.RecordHeader(CheckId, NxFr::Stats::StatType::Check, NxFr::Stats::StatMode::Set);
		Stats.RecordHeader(SetId, NxFr::Stats::StatType::Integer, NxFr::Stats::StatMode::Set);
		Stats.RecordHeader(MinId, NxFr::Stats::StatType::Integer, NxFr::Stats::StatMode::Min);
		Stats.RecordHeader(MaxId, NxFr::Stats::StatType::Decimal, NxFr::Stats::StatMode::Max);
		Stats.RecordHeader(AvgId, NxFr::Stats::StatType::Decimal, NxFr::Stats::StatMode::Avg);
		Stats.RecordHeader(CntId, NxFr::Stats::StatType::Integer, NxFr::Stats::StatMode::Cnt);
		Stats.RecordHeader(AddId, NxFr::Stats::StatType::Integer, NxFr::Stats::StatMode::Add);

		Stats.Initialize();
		ASSERT_EQ(Stats.IsInitialized(), true);
		ASSERT_EQ(Stats.GetCount(), 10);

		Stats.StartRecording();
		ASSERT_EQ(Stats.IsRecording(), true);

		for (uint64 Iteration = 1; Iteration <= 5; Iteration++)
		{
			Stats.RecordStatLabel(LabelId, NxFr::StringView("Test"));
			Stats.RecordStatCheck(CheckId, Iteration % 2);
			Stats.RecordStatInteger(SetId, Iteration);
			Stats.RecordStatInteger(MinId, Iteration);
			Stats.RecordStatDecimal(MaxId, (float)Iteration);
			Stats.RecordStatDecimal(AvgId, (double)Iteration);
			Stats.RecordStatInteger(CntId, 0);
			Stats.RecordStatInteger(AddId, 2);

			Stats.RecordComment(NxFr::StringUtility::FormatTo("Iteration: %d", Iteration));
			Stats.RecordComment(NxFr::StringUtility::FormatTo("Iteration (Again): %d", Iteration));

			ASSERT_EQ(Stats.GetStat(SetId).GetValueInteger(), Iteration);

			Stats.Flush();
			if (Iteration == 3)
			{
				Stats.Reset();
				ASSERT_EQ(Stats.GetStat(NxFr::StatsHeader::TickId).GetValueInteger(), 0);
			}
		}

		Stats.StopRecording();
		ASSERT_EQ(!Stats.IsRecording(), true);

		NxFr::Dictionary<NxFr::StringId, const NxFr::Stats::Stat*> All = Stats.GetStats();
		ASSERT_EQ(All.GetCount(), Stats.GetCount());
	}

	TEST(Debug, Instruments)
	{
		NxFr::String Path = Dummy::GeneratePath("Debug_Instruments.json");
		NxFr::Delegate<void(uint64, NxFr::Instruments*)> Dummy = [](uint64 Count, NxFr::Instruments* Instruments)
		{
			NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);
			NxFr::Thread::Sleep(Count);
		};

		NxFr::ChromeTracing Instruments = NxFr::ChromeTracing(Path);
		Instruments.StartRecording();

		{
			NxFr::Instruments::Marker Marker(__FUNCSIG__, &Instruments);

			Dummy(5, &Instruments);
			Dummy(10, &Instruments);
		}

		Instruments.StopRecording();
	}
}
