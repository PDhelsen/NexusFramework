#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr
{
	namespace Decimal
	{
		constexpr float MinF = NEXUS_DECIMAL_MIN_FLOAT;
		constexpr double MinD = NEXUS_DECIMAL_MIN_DOUBLE;
		constexpr float MaxF = NEXUS_DECIMAL_MAX_FLOAT;
		constexpr double MaxD = NEXUS_DECIMAL_MAX_DOUBLE;
		constexpr float InfinityF = NEXUS_DECIMAL_INF_FLOAT;
		constexpr double InfinityD = NEXUS_DECIMAL_INF_DOUBLE;
		constexpr float EpsilonF = NEXUS_DECIMAL_EPS_FLOAT;
		constexpr double EpsilonD = NEXUS_DECIMAL_EPS_DOUBLE;
	}
}
