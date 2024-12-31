#include "Core/NexusEnginePch.h"
#include "Decimal.h"

namespace NxEn::Decimal
{
	constexpr float InfinityF()
	{
		return INFINITY;
	}

	constexpr double InfinityD()
	{
		return INFINITY;
	}

	constexpr float MinF()
	{
		return FLT_MIN;
	}

	constexpr double MinD()
	{
		return DBL_MIN;
	}

	constexpr float MaxF()
	{
		return FLT_MAX;
	}

	constexpr double MaxD()
	{
		return DBL_MAX;
	}
}
