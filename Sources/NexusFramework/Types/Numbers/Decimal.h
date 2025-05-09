#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr::Decimal
{
	constexpr float InfinityF = INFINITY;
	constexpr double InfinityD = INFINITY;
	constexpr float MinF = FLT_MIN;
	constexpr double MinD = DBL_MIN;
	constexpr float MaxF = FLT_MAX;
	constexpr double MaxD = DBL_MAX;
	constexpr float EpsilonF = 1e-6f;
	constexpr double EpsilonD = 1e-9f;
}
