#include "Core/NexusEnginePch.h"
#include "Allocator.h"

namespace NxEn
{
	Allocator::Allocator(uint64 Size)
		: Capacity(Size), Amount(0)
	{
		Memory = Memory::Malloc(Capacity);
#if NEXUS_DEBUG
		Memory::MemSet(Memory, 0, Capacity);
#endif
	}
	
	Allocator::~Allocator()
	{
		Memory::Free(Memory);
	}
}