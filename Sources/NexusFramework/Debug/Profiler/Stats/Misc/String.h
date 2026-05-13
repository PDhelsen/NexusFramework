#pragma once

#include "NexusFramework/Debug/Profiler/Stats/Stats.h"

#include "NexusFramework/Types/Strings/Utility.h"

namespace NxFr
{
	namespace StringUtility
	{
		template<>
		struct Converter<Stats::Stat>
		{
			static void ToString(const Stats::Stat& Data, String& Result, StringView Format = "")
			{
				switch (Data.GetType())
				{
				case NxFr::Stats::Type::Label: StringUtility::ToString(Data.GetValueLabel(), Result); break;
				case NxFr::Stats::Type::Check: StringUtility::ToString(Data.GetValueCheck(), Result); break;
				case NxFr::Stats::Type::Integer: StringUtility::ToString(Data.GetValueInteger(), Result); break;
				case NxFr::Stats::Type::Decimal: StringUtility::ToString(Data.GetValueDecimal(), Result); break;
				}
			}
		};
	}
}
