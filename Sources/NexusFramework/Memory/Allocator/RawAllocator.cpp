#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/RawAllocator.h"

namespace NxFr
{
	RawAllocator::RawAllocator()
		: Allocator(0), Allocated(11, nullptr)
	{
	}

	RawAllocator::~RawAllocator()
	{
	}

	void RawAllocator::Clear()
	{
	}

	bool RawAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		return true;
	}

	bool RawAllocator::BelongToAllocator(void* Pointer) const
	{
		return Allocated.TryGet(Pointer) != nullptr;
	}

	void* RawAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		void* Pointer = Memory::Allocate(Size, nullptr, Alignement);
		Allocated.Append(Pointer);
		return Pointer;
	}

	void* RawAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (Pointer == nullptr)
		{
			return nullptr;
		}

		NEXUS_ASSERT(BelongToAllocator(Pointer), Default, "Memory was not allocated from this allocator");

		Allocated.Remove(Pointer);
		Pointer = Memory::Reallocate(Pointer, Size, nullptr, Alignement);
		Allocated.Append(Pointer);

		return Pointer;
	}

	void RawAllocator::Free(void* Pointer)
	{
		if (Pointer == nullptr)
		{
			return;
		}

		NEXUS_ASSERT(BelongToAllocator(Pointer), Default, "Memory was not allocated from this allocator");

		Memory::Free(Pointer, nullptr);
		Allocated.Remove(Pointer);
	}
}
