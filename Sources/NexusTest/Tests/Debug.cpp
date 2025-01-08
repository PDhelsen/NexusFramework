#include "Core/NexusTestPch.h"

namespace NxTs
{
	NxEn::StringId Channel = "Test"_Sid;

	NxEn::StringId LabelId = "Label"_Sid;
	NxEn::StringId CheckId = "Check"_Sid;
	NxEn::StringId SetId = "Set"_Sid;
	NxEn::StringId MinId = "Min"_Sid;
	NxEn::StringId MaxId = "Max"_Sid;
	NxEn::StringId AvgId = "Avg"_Sid;
	NxEn::StringId CntId = "Cnt"_Sid;
	NxEn::StringId AddId = "Add"_Sid;

	void Dummy(uint64 Count, NxEn::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		NxEn::Platform::GetInstance()->Sleep(Count);
	}

	void Function1(uint64 Count, NxEn::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		Dummy(Count, Instruments);
	}

	void Function2(uint64 Count, NxEn::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		Dummy(Count, Instruments);
		Function1(Count * 2, Instruments);
	}

	TEST(Debug, Logger)
	{
		NxEn::Path Path = NxEn::Path::GetWorkingDirectory() + "Logs.txt";

		{
			NxEn::Logger Logger = NxEn::Logger(false, NxEn::LoggerVerbosity::All, NxEn::LoggerOutput::All, Path);

			Logger.AddChannel(Channel, false);
			ASSERT_EQ(Logger.HasChannel(Channel), true);
			ASSERT_EQ(Logger.CheckChannel(Channel), false);
			Logger.SetChannel(Channel, true);
			ASSERT_EQ(Logger.CheckChannel(Channel), true);

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

			Logger.LogMessage(NxEn::LoggerVerbosity::Info, Channel, "Test");
			Logger.LogMessage(NxEn::LoggerVerbosity::Warning, Channel, "Test: %d", 10);
			Logger.LogMessage(NxEn::LoggerVerbosity::Error, Channel, "Test: %f", 10.0f);
			Logger.LogMessage(NxEn::LoggerVerbosity::Fatal, Channel, "Test: %s", "Hello World");
		}

		NxEn::File File = NxEn::File(Path);
		ASSERT_EQ(File.Exists(), true);
		File.Delete();
	}

	TEST(Debug, Stats)
	{
		NxEn::Path Path = NxEn::Path::GetWorkingDirectory() + "Stats.csv";

		{
			NxEn::Stats Stats(Path);
			ASSERT_EQ(NxEn::Path::Exist(Stats.GetPath()), true);

			Stats.RecordHeader(LabelId, NxEn::Stats::StatType::Label, NxEn::Stats::StatMode::Set);
			Stats.RecordHeader(CheckId, NxEn::Stats::StatType::Check, NxEn::Stats::StatMode::Set);
			Stats.RecordHeader(SetId, NxEn::Stats::StatType::Integer, NxEn::Stats::StatMode::Set);
			Stats.RecordHeader(MinId, NxEn::Stats::StatType::UnsignedInteger, NxEn::Stats::StatMode::Min);
			Stats.RecordHeader(MaxId, NxEn::Stats::StatType::Decimal, NxEn::Stats::StatMode::Max);
			Stats.RecordHeader(AvgId, NxEn::Stats::StatType::DecimalPrecision, NxEn::Stats::StatMode::Avg);
			Stats.RecordHeader(CntId, NxEn::Stats::StatType::UnsignedInteger, NxEn::Stats::StatMode::Cnt);
			Stats.RecordHeader(AddId, NxEn::Stats::StatType::Integer, NxEn::Stats::StatMode::Add);

			Stats.Initialize();
			ASSERT_EQ(Stats.IsInitialized(), true);
			ASSERT_EQ(Stats.GetCount(), 10);

			Stats.StartRecording();
			ASSERT_EQ(Stats.IsRecording(), true);

			for (uint64 Iteration = 1; Iteration <= 5; Iteration++)
			{
				Stats.Lock();
				ASSERT_EQ(Stats.IsLocked(), true);

				Stats.RecordStatLabel(LabelId, NxEn::StringView("Test"));
				Stats.RecordStatCheck(CheckId, Iteration % 2);
				Stats.RecordStatInteger(SetId, Iteration);
				Stats.RecordStatUnsignedInteger(MinId, Iteration);
				Stats.RecordStatDecimal(MaxId, (float)Iteration);
				Stats.RecordStatDecimalPrecision(AvgId, (double)Iteration);
				Stats.RecordStatUnsignedInteger(CntId, 0);
				Stats.RecordStatInteger(AddId, 2);

				Stats.RecordComment(NxEn::StringUtility::Format("Iteration: %d", Iteration));
				Stats.RecordComment(NxEn::StringUtility::Format("Iteration (Again): %d", Iteration));

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

		NxEn::File File = NxEn::File(Path);
		ASSERT_EQ(File.Exists(), true);
		File.Delete();
	}

	TEST(Debug, Instruments)
	{
		NxEn::Path Path = NxEn::Path::GetWorkingDirectory() + "Instruments.json";

		{
			NxEn::Instruments* Instruments = NxEn::Instruments::Create(Path);
			Instruments->StartRecording();

			{
				NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

				Function1(5, Instruments);
				Function2(10, Instruments);
			}

			Instruments->StopRecording();
			NxEn::Instruments::Destroy(Instruments);
		}

		NxEn::File File = NxEn::File(Path);
		ASSERT_EQ(File.Exists(), true);
		File.Delete();
	}
}
