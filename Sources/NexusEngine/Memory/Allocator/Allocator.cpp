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
		: Capacity(Size), Amount(0)
	{
		Memory = Memory::Allocate(Capacity);
	}
	
	Allocator::~Allocator()
	{
		Memory::Free(Memory);
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
}
