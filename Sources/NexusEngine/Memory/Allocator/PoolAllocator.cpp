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

	void* PoolAllocator::Allocate()
	{
		NEXUS_ASSERT(Head != nullptr, "Head is null")
		NEXUS_ASSERT(*Head != 0, "Next head is null")
		NEXUS_ASSERT(CanAllocate(), "Pool is Full")

		void* Pointer = Head;
		
		Head = reinterpret_cast<uint64*>(*Head);
		if (*Head == 0)
		{
			*Head = reinterpret_cast<uint64>(Head) + Stride;
		}
		
		UpdateAmount(Stride, true);

		return Pointer;
	}

	void PoolAllocator::Free(void* Pointer)
	{
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
	
	bool PoolAllocator::CanAllocate() const
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