#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	namespace Decimal
	{
		inline constexpr float MinF = NEXUS_DECIMAL_MIN_FLOAT;
		inline constexpr double MinD = NEXUS_DECIMAL_MIN_DOUBLE;
		inline constexpr float MaxF = NEXUS_DECIMAL_MAX_FLOAT;
		inline constexpr double MaxD = NEXUS_DECIMAL_MAX_DOUBLE;
		inline constexpr float InfinityF = NEXUS_DECIMAL_INF_FLOAT;
		inline constexpr double InfinityD = NEXUS_DECIMAL_INF_DOUBLE;
		inline constexpr float EpsilonF = NEXUS_DECIMAL_EPS_FLOAT;
		inline constexpr double EpsilonD = NEXUS_DECIMAL_EPS_DOUBLE;
	}
}
