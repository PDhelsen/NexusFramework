#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/SystemAllocator.h"

namespace NxFr
{
	SystemAllocator::SystemAllocator()
		: Allocator(0), Allocated(11, nullptr)
	{
	}

	SystemAllocator::~SystemAllocator()
	{
	}

	void SystemAllocator::Clear()
	{
	}

	bool SystemAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		return true;
	}

	bool SystemAllocator::BelongToAllocator(void* Pointer) const
	{
		return Allocated.TryGet(Pointer) != nullptr;
	}

	void* SystemAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		void* Pointer = Memory::Allocate(Size, nullptr, Alignement);
		Allocated.Append(Pointer);
		return Pointer;
	}

	void* SystemAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (Pointer == nullptr)
		{
			return nullptr;
		}

		NX_ASSERT(BelongToAllocator(Pointer), Default, "Memory was not allocated from this allocator");

		Allocated.Remove(Pointer);
		Pointer = Memory::Reallocate(Pointer, Size, nullptr, Alignement);
		Allocated.Append(Pointer);

		return Pointer;
	}

	void SystemAllocator::Free(void* Pointer)
	{
		if (Pointer == nullptr)
		{
			return;
		}

		NX_ASSERT(BelongToAllocator(Pointer), Default, "Memory was not allocated from this allocator");

		Memory::Free(Pointer, nullptr);
		Allocated.Remove(Pointer);
	}
}
