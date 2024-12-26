#include "Core/NexusEnginePch.h"
#include "PoolAllocator.h"

namespace NxEn
{
	PoolAllocator::PoolAllocator(uint64 Count, uint64 Stride)
		: Allocator(Count * Stride), Head(nullptr), Stride(Stride)
	{
		NEXUS_ASSERT(Stride >= sizeof(void*), "Element size should be at least : %d bytes", sizeof(void*));

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
		if (FreeAmount() < Stride)
		{
			NEXUS_ASSERT(false, "Allocator is full");
			return nullptr;
		}

		void* Pointer = Head;
		
		Next();

		IncreaseAmount(Stride);
		return Pointer;
	}

	void* PoolAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(false, "Reallocate from Pool Allocator is not supported")
		return nullptr;
	}

	void PoolAllocator::Free(void* Pointer)
	{
		if (!Pointer || !IsPointerInMemoryBlock(Pointer))
		{
			return;
		}

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
