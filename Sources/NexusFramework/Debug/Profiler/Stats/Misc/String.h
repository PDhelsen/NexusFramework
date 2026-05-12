#pragma once

#include "NexusFramework/Types/Strings/Utility.h"

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
			case NxFr::Stats::Type::Label: StringConverter<String>::ToString(Data.GetValueLabel(), Result); break;
			case NxFr::Stats::Type::Check: StringConverter<bool>::ToString(Data.GetValueCheck(), Result); break;
			case NxFr::Stats::Type::Integer: StringConverter<int64>::ToString(Data.GetValueInteger(), Result); break;
			case NxFr::Stats::Type::Decimal: StringConverter<float>::ToString(Data.GetValueDecimal(), Result); break;
			}
		}
	};
}
