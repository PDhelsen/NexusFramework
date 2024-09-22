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
}
