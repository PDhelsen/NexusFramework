#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	static Stack<Allocator*>& GetAllocators() { static thread_local Stack<Allocator*> Allocators(nullptr); return Allocators; }

	Allocator* Allocator::Scope::Get()
	{
		Stack<Allocator*>& Allocators = GetAllocators();

		if (Allocators.GetCount() == 0)
		{
			return nullptr;
		}

		return Allocators.Get();
	}

	void Allocator::Scope::Push(Allocator* Alloc)
	{
		Stack<Allocator*>& Allocators = GetAllocators();
		Allocators.Append(Alloc);
	}

	void Allocator::Scope::Pop()
	{
		Stack<Allocator*>& Allocators = GetAllocators();
		Allocators.Remove();
	}

	void Allocator::Scope::Reset()
	{
		Stack<Allocator*>& Allocators = GetAllocators();
		Allocators.Clear();
	}

	Allocator::Scope::Scope(Allocator* Allocator)
	{
		Allocator::Scope::Push(Allocator);
	}

	Allocator::Scope::~Scope()
	{
		Allocator::Scope::Pop();
	}

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
