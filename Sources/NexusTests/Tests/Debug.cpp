#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	NxFr::StringId Channel = "Test"_Sid;

	NxFr::StringId LabelId = "Label"_Sid;
	NxFr::StringId CheckId = "Check"_Sid;
	NxFr::StringId SetId = "Set"_Sid;
	NxFr::StringId MinId = "Min"_Sid;
	NxFr::StringId MaxId = "Max"_Sid;
	NxFr::StringId AvgId = "Avg"_Sid;
	NxFr::StringId CntId = "Cnt"_Sid;
	NxFr::StringId AddId = "Add"_Sid;

	void Dummy(uint64 Count, NxFr::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		NxFr::Thread::Sleep(Count);
	}

	void Function1(uint64 Count, NxFr::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		Dummy(Count, Instruments);
	}

	void Function2(uint64 Count, NxFr::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		Dummy(Count, Instruments);
		Function1(Count * 2, Instruments);
	}

	TEST(Debug, Logger)
	{
		NxFr::Path Path = NxFr::Path::GetWorkingDirectory() + "Logs.txt";

		{
			NxFr::Logger Logger = NxFr::Logger(false, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, Path);

			Logger.AddChannel(Channel, false);
			ASSERT_EQ(Logger.HasChannel(Channel), true);
			ASSERT_EQ(Logger.CheckChannel(Channel), false);
			Logger.SetChannel(Channel, true);
			ASSERT_EQ(Logger.CheckChannel(Channel), true);

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

			Logger.LogMessage(NxFr::LoggerVerbosity::Info, Channel, "Test");
			Logger.LogMessage(NxFr::LoggerVerbosity::Warning, Channel, "Test: %d", 10);
			Logger.LogMessage(NxFr::LoggerVerbosity::Error, Channel, "Test: %f", 10.0f);
			Logger.LogMessage(NxFr::LoggerVerbosity::Fatal, Channel, "Test: %s", "Hello World");
		}

		NxFr::File File = NxFr::File(Path);
		ASSERT_EQ(File.Exists(), true);
		File.Delete();
	}

	TEST(Debug, Stats)
	{
		NxFr::Path Path = NxFr::Path::GetWorkingDirectory() + "Stats.csv";

		{
			NxFr::Stats Stats(Path);
			ASSERT_EQ(NxFr::Path::Exist(Stats.GetPath()), true);

			Stats.RecordHeader(LabelId, NxFr::Stats::StatType::Label, NxFr::Stats::StatMode::Set);
			Stats.RecordHeader(CheckId, NxFr::Stats::StatType::Check, NxFr::Stats::StatMode::Set);
			Stats.RecordHeader(SetId, NxFr::Stats::StatType::Integer, NxFr::Stats::StatMode::Set);
			Stats.RecordHeader(MinId, NxFr::Stats::StatType::UnsignedInteger, NxFr::Stats::StatMode::Min);
			Stats.RecordHeader(MaxId, NxFr::Stats::StatType::Decimal, NxFr::Stats::StatMode::Max);
			Stats.RecordHeader(AvgId, NxFr::Stats::StatType::DecimalPrecision, NxFr::Stats::StatMode::Avg);
			Stats.RecordHeader(CntId, NxFr::Stats::StatType::UnsignedInteger, NxFr::Stats::StatMode::Cnt);
			Stats.RecordHeader(AddId, NxFr::Stats::StatType::Integer, NxFr::Stats::StatMode::Add);

			Stats.Initialize();
			ASSERT_EQ(Stats.IsInitialized(), true);
			ASSERT_EQ(Stats.GetCount(), 10);

			Stats.StartRecording();
			ASSERT_EQ(Stats.IsRecording(), true);

			for (uint64 Iteration = 1; Iteration <= 5; Iteration++)
			{
				Stats.Lock();
				ASSERT_EQ(Stats.IsLocked(), true);

				Stats.RecordStatLabel(LabelId, NxFr::StringView("Test"));
				Stats.RecordStatCheck(CheckId, Iteration % 2);
				Stats.RecordStatInteger(SetId, Iteration);
				Stats.RecordStatUnsignedInteger(MinId, Iteration);
				Stats.RecordStatDecimal(MaxId, (float)Iteration);
				Stats.RecordStatDecimalPrecision(AvgId, (double)Iteration);
				Stats.RecordStatUnsignedInteger(CntId, 0);
				Stats.RecordStatInteger(AddId, 2);

				Stats.RecordComment(NxFr::StringUtility::Format("Iteration: %d", Iteration));
				Stats.RecordComment(NxFr::StringUtility::Format("Iteration (Again): %d", Iteration));

				Stats.Unlock();
				ASSERT_EQ(!Stats.IsLocked(), true);

				ASSERT_EQ(Stats.GetCurrentStatValue<int64>(SetId), Iteration);

				Stats.Flush();
				if (Iteration == 3)
				{
					Stats.Reset();
				}
			}

			Stats.StopRecording();
			ASSERT_EQ(!Stats.IsRecording(), true);

			auto All = Stats.GetAllCurrentStats();
			ASSERT_EQ(All.GetCount(), Stats.GetCount());
		}

		NxFr::File File = NxFr::File(Path);
		ASSERT_EQ(File.Exists(), true);
		File.Delete();
	}

	TEST(Debug, Instruments)
	{
		NxFr::Path Path = NxFr::Path::GetWorkingDirectory() + "Instruments.json";

		{
			NxFr::Instruments* Instruments = NxFr::Instruments::Create(Path);
			Instruments->StartRecording();

			{
				NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

				Function1(5, Instruments);
				Function2(10, Instruments);
			}

			Instruments->StopRecording();
			NxFr::Instruments::Destroy(Instruments);
		}

		NxFr::File File = NxFr::File(Path);
		ASSERT_EQ(File.Exists(), true);
		File.Delete();
	}
}
