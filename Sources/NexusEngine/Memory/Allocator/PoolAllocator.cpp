#include "Core/NexusEnginePch.h"
#include "PoolAllocator.h"

namespace NxEn
{
	PoolAllocator::PoolAllocator(uint64 Count, uint64 Stride)
		: Allocator(Count * Stride), Stride(Stride)
	{
		NEXUS_ASSERT(Stride >= sizeof(void*), "Element size should be at least : %d bytes", sizeof(void*))

		Clear();
	}

	PoolAllocator::~PoolAllocator()
	{
	}

	//TODO: Implementation - PoolAllocator - Check behaviour when allocating last element
	void* PoolAllocator::Allocate(uint64 Size /* 0 */, uint64 Alignement /* 0 */)
	{
		NEXUS_ASSERT(Stride == Size || Size == 0, "Requested size is different from pool stride (it may be because the request is an array but it is not supported).")
		NEXUS_ASSERT(Head != nullptr, "Head is null")
		NEXUS_ASSERT(*Head != 0, "Next head is null")
		NEXUS_ASSERT(CanAllocate(), "Pool is Full")

		void* Pointer = Head;
		
		Head = reinterpret_cast<uint64*>(*Head);
		if (Head != nullptr && *Head == 0)
		{
			*Head = reinterpret_cast<uint64>(Head) + Stride;
		}
		
		UpdateAmount(Stride, true);

		return Pointer;
	}

	void* PoolAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(false, "Reallocate from Pool Allocator is not supported")
		return nullptr;
	}

	void PoolAllocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		NEXUS_ASSERT(Head != nullptr, "Head is null")
		NEXUS_ASSERT(*Head != 0, "Next head is null")
		NEXUS_ASSERT(IsValidAddress(Pointer), "Address is outside of the pool")

		EraseMemory(Pointer, Stride);
		UpdateAmount(Stride, false);

		uint64 Address = reinterpret_cast<uint64>(Head);
		Head = (uint64*)Pointer;
		*Head = Address;
	}

	void PoolAllocator::Clear()
	{
		WipeoutMemory();
		ResetAmount();

		Head = (uint64*)GetMemoryBlock();;
		*Head = reinterpret_cast<uint64>(Head) + Stride;
	}
	
	bool PoolAllocator::CanAllocate(uint64 Size /* 0 */, uint64 Alignement /* 0 */) const
	{
		return SlotAvailable() > 0;
	}

	bool PoolAllocator::IsValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")

		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(GetMemoryBlock());
		uint64 End = Start + TotalAmount();

		return Address >= Start && Address < End;
	}

	uint64 PoolAllocator::SlotAvailable() const
	{
		return FreeAmount() / Stride;
	}
}