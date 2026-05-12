#pragma once

#include "NexusFramework/Types/Strings/StringFunctions.h"
#include "NexusFramework/Debug/Profiler/Stats/Stats.h"

namespace NxFr
{
	template<>
	struct StringConverter<Stats::Stat>
	{
		static void ToString(const Stats::Stat& Data, String& Result, StringView Format = "")
		{
			switch (Data.GetType())
			{
			case NxFr::Stats::StatType::Label: StringConverter<String>::ToString(Data.GetValueLabel(), Result); break;
			case NxFr::Stats::StatType::Check: StringConverter<bool>::ToString(Data.GetValueCheck(), Result); break;
			case NxFr::Stats::StatType::Integer: StringConverter<int64>::ToString(Data.GetValueInteger(), Result); break;
			case NxFr::Stats::StatType::Decimal: StringConverter<float>::ToString(Data.GetValueDecimal(), Result); break;
			}
		}
	};
}
