#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numbers/Decimal.h"

#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr::Decimal
{
	float InfinityF()
	{
		return INFINITY;
	}

	double InfinityD()
	{
		return INFINITY;
	}

	float MinF()
	{
		return FLT_MIN;
	}

	double MinD()
	{
		return DBL_MIN;
	}

	float MaxF()
	{
		return FLT_MAX;
	}

	double MaxD()
	{
		return DBL_MAX;
	}

	float EpsilonF()
	{
		return 1e-6f;
	}

	double EpsilonD()
	{
		return 1e-9f;
	}
}
