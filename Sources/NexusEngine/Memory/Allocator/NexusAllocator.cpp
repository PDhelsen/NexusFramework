#include "Core/NexusEnginePch.h"
#include "NexusAllocator.h"

namespace NxEn
{
	NexusAllocator::NexusAllocator(uint64 Size)
		: Capacity(Size), Amount(0)
	{
		Memory = Memory::Allocate(Capacity);
	}

	NexusAllocator::~NexusAllocator()
	{
		Memory::Free(Memory);
	}

	void NexusAllocator::WipeoutMemory()
	{
		Memory::MemSet(Memory, 0, Capacity);
	}

	void NexusAllocator::EraseMemory(void* Memory, uint64 Size)
	{
#if NEXUS_DEBUG
		Memory::MemSet(Memory, 0, Size);
#endif
	}

	bool NexusAllocator::IsPointerInside(void* Pointer) const
	{
		return Memory::IsPointerInRange(Pointer, GetMemoryBlock(), TotalAmount());
	}
}
