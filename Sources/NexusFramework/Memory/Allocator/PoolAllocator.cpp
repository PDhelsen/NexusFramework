#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/PoolAllocator.h"

namespace NxFr
{
	PoolAllocator::PoolAllocator(uint64 Size, uint64 Stride)
		: MemoryAllocator(Size), Head(nullptr), Stride(Stride)
	{
		NEXUS_ASSERT(Stride >= sizeof(void*), Default, "Element size should be at least : %llu bytes", sizeof(void*));
		NEXUS_ASSERT(Math::IsMultiple(Size, Stride), Default, "Size (%llu) is not a multiple of the stride (%llu)", Size, Stride);

		Clear();
	}

	PoolAllocator::~PoolAllocator()
	{
	}

	void PoolAllocator::Clear()
	{
		WipeoutMemory();
		ResetAmount();

		Reset();
	}

	bool PoolAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		return FreeAmount() >= Stride;
	}

	bool PoolAllocator::BelongToAllocator(void* Pointer) const
	{
		return Pointer && IsPointerInMemoryBlock(Pointer);
	}

	void* PoolAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(FreeAmount() >= Stride, Default, "Allocator is full");

		void* Pointer = Head;

		Next();

		IncreaseAmount(Stride);
		return Pointer;
	}

	void* PoolAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(false, Default, "Reallocate from Pool Allocator is not supported");
		return nullptr;
	}

	void PoolAllocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		NEXUS_ASSERT(IsPointerInMemoryBlock(Pointer), Default, "Invalid pointer");

		EraseMemory(Pointer, Stride);

		Previous(Pointer);

		DecreaseAmount(Stride);
	}

	void PoolAllocator::Next()
	{
		Head = reinterpret_cast<uint64*>(*Head);
		if (Head != nullptr && IsPointerInMemoryBlock(Head) && *Head == 0)
		{
			*Head = reinterpret_cast<uint64>(Head) + Stride;
		}
	}

	void PoolAllocator::Previous(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Head);
		Head = (uint64*)Pointer;
		*Head = Address;
	}

	void PoolAllocator::Reset()
	{
		Head = (uint64*)GetMemoryBlock();;
		*Head = reinterpret_cast<uint64>(Head) + Stride;
	}
}
