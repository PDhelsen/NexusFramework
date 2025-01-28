#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numbers/Decimal.h"

#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr::Decimal
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
