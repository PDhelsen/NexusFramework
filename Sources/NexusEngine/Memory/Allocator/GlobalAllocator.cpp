#include "Core/NexusEnginePch.h"
#include "GlobalAllocator.h"

namespace NxEn
{
	void* GlobalAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		return Memory::Allocate(Size, nullptr, Alignement);
	}

	void* GlobalAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		return Memory::Reallocate(Pointer, Size, nullptr, Alignement);
	}

	void GlobalAllocator::Free(void* Pointer)
	{
		return Memory::Free(Pointer, nullptr);
	}
}
