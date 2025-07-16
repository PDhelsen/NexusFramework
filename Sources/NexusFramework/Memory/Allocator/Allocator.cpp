#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	Allocator::Allocator(uint64 Size)
		: Amount(0), Capacity(Size)
	{
	}

	void Allocator::IncreaseAmount(uint64 Delta)
	{
		NEXUS_ASSERT(Capacity == 0 || Amount + Delta <= Capacity, Default, "Invalid delta");

		Amount += Delta;
	}

	void Allocator::DecreaseAmount(uint64 Delta)
	{
		NEXUS_ASSERT(Amount >= Delta, Default, "Invalid delta");

		Amount -= Delta;
	}

	void Allocator::ResetAmount()
	{
		Amount = 0;
	}
}
