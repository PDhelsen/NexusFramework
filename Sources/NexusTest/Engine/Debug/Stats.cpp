#include "Core/NexusTestPch.h"

#include "Debug/Profiler/Stats/Stats.h"
#include "Misc/IO/Path.h"

namespace NxTs
{
	TEST(Stats, Stats)
	{
		NxEn::StringId LabelId = "Label"_Sid;
		NxEn::StringId CheckId = "Check"_Sid;
		NxEn::StringId SetId = "Set"_Sid;
		NxEn::StringId MinId = "Min"_Sid;
		NxEn::StringId MaxId = "Max"_Sid;
		NxEn::StringId AvgId = "Avg"_Sid;
		NxEn::StringId CntId = "Cnt"_Sid;

		NxEn::Stats Stats(NxEn::Path::GetWorkingDirectory() + "Stats.csv");
		ASSERT_EQ(NxEn::Path::Exist(Stats.GetPath()), true);

		Stats.RecordHeader(LabelId, NxEn::Stats::StatType::Label, NxEn::Stats::StatMode::Set);
		Stats.RecordHeader(CheckId, NxEn::Stats::StatType::Check, NxEn::Stats::StatMode::Set);
		Stats.RecordHeader(SetId, NxEn::Stats::StatType::Integer, NxEn::Stats::StatMode::Set);
		Stats.RecordHeader(MinId, NxEn::Stats::StatType::UnsignedInteger, NxEn::Stats::StatMode::Min);
		Stats.RecordHeader(MaxId, NxEn::Stats::StatType::Decimal, NxEn::Stats::StatMode::Max);
		Stats.RecordHeader(AvgId, NxEn::Stats::StatType::DecimalPrecision, NxEn::Stats::StatMode::Avg);
		Stats.RecordHeader(CntId, NxEn::Stats::StatType::UnsignedInteger, NxEn::Stats::StatMode::Cnt);

		Stats.Initialize();
		ASSERT_EQ(Stats.IsInitialized(), true);
		ASSERT_EQ(Stats.GetCount(), 9);

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
			Stats.RecordStatCount(CntId);

			Stats.RecordComment(NxEn::StringUtility::Format("Iteration: %d", Iteration));
			Stats.RecordComment(NxEn::StringUtility::Format("Iteration (Again): %d", Iteration));

			Stats.Unlock();
			ASSERT_EQ(!Stats.IsLocked(), true);

			ASSERT_EQ(Stats.GetCurrentTick(), Iteration);
			ASSERT_EQ(Stats.GetCurrentComment(), NxEn::StringUtility::Format("Iteration: %d", Iteration) + ";" + NxEn::StringUtility::Format("Iteration (Again): %d", Iteration) + ";");
			ASSERT_EQ(Stats.GetCurrentStat(SetId)->GetValue().Integer, Iteration);

			Stats.Flush();
			if (Iteration == 3)
			{
				Stats.Reset();
				ASSERT_EQ(Stats.GetSpan(), 1.0);
			}
		}

		Stats.StopRecording();
		ASSERT_EQ(!Stats.IsRecording(), true);

		auto All = Stats.GetAllCurrentStats();
		ASSERT_EQ(All.GetCount(), Stats.GetCount());
	}
}
