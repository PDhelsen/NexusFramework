#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	namespace Decimal
	{
		inline constexpr float MinF = NX_DECIMAL_MIN_FLOAT;
		inline constexpr double MinD = NX_DECIMAL_MIN_DOUBLE;
		inline constexpr float MaxF = NX_DECIMAL_MAX_FLOAT;
		inline constexpr double MaxD = NX_DECIMAL_MAX_DOUBLE;
		inline constexpr float InfinityF = NX_DECIMAL_INF_FLOAT;
		inline constexpr double InfinityD = NX_DECIMAL_INF_DOUBLE;
		inline constexpr float EpsilonF = NX_DECIMAL_EPS_FLOAT;
		inline constexpr double EpsilonD = NX_DECIMAL_EPS_DOUBLE;
	}
}
