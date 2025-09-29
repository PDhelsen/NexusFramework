#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr::Decimal
{
#ifndef __INTELLISENSE__
	constexpr float InfinityF = INFINITY;
	constexpr double InfinityD = INFINITY;
#endif

	constexpr float MinF = FLT_MIN;
	constexpr double MinD = DBL_MIN;
	constexpr float MaxF = FLT_MAX;
	constexpr double MaxD = DBL_MAX;
	constexpr float EpsilonF = 1e-4f;
	constexpr double EpsilonD = 1e-9f;
}
