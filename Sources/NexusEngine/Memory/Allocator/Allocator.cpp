#include "Core/NexusEnginePch.h"
#include "Allocator.h"

namespace NxEn
{
	AllocatorActive::AllocatorActive(Allocator* Allocator)
	{
		Memory::PushActiveAllocator(Allocator);
	}

	AllocatorActive::~AllocatorActive()
	{
		Memory::PopActiveAllocator();
	}

	Allocator::Allocator(uint64 Size)
		: Capacity(Size), Amount(0), Memory(nullptr)
	{
		Memory = Memory::Allocate(Capacity, nullptr);
	}

	Allocator::~Allocator()
	{
		Memory::Free(Memory, nullptr);
	}

	void Allocator::IncreaseAmount(uint64 Delta)
	{
		NEXUS_ASSERT(Amount + Delta <= Capacity, "Invalid delta");

		Amount += Delta;
	}

	void Allocator::DecreaseAmount(uint64 Delta)
	{
		NEXUS_ASSERT(Amount > Delta, "Invalid delta");

		Amount -= Delta;
	}

	void Allocator::ResetAmount()
	{
		Amount = 0;
	}

	void Allocator::WipeoutMemory()
	{
		Memory::MemSet(Memory, 0, Capacity);
	}

	void Allocator::EraseMemory(void* Memory, uint64 Size)
	{
#if NEXUS_DEBUG
		Memory::MemSet(Memory, 0, Size);
#endif
	}

	bool Allocator::IsPointerInMemoryBlock(void* Pointer) const
	{
		return Memory::IsPointerInRange(Pointer, Memory, Capacity);
	}

	void* Allocator::GetMemoryBlock() const
	{
		return Memory;
	}
}
