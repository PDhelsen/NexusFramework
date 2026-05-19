#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Pattern/Context.h"

namespace NxFr
{
	Allocator::Scope::Scope(Allocator* Instance)
	{
		Allocator::GetContexts().Push(Instance);
	}

	Allocator::Scope::~Scope()
	{
		Allocator::GetContexts().Pop();
	}

	Context<Allocator>& Allocator::GetContexts()
	{
		static thread_local Context<Allocator> Contexts;
		return Contexts;
	}

	Allocator* Allocator::TryGet()
	{
		return Allocator::GetContexts().TryGet();
	}

	Allocator::Allocator(uint64 Size)
		: Amount(0), Capacity(Size)
	{
	}

	void Allocator::IncreaseAmount(uint64 Delta)
	{
		NX_ASSERT(Capacity == 0 || Amount + Delta <= Capacity, Default, "Invalid delta");

		Amount += Delta;
	}

	void Allocator::DecreaseAmount(uint64 Delta)
	{
		NX_ASSERT(Amount >= Delta, Default, "Invalid delta");

		Amount -= Delta;
	}

	void Allocator::ResetAmount()
	{
		Amount = 0;
	}
}
