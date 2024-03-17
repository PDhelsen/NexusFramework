#include "Core/NexusEnginePch.h"
#include "PoolAllocator.h"

namespace NxEn
{
	PoolAllocator::PoolAllocator(uint64 Count, uint64 Stride)
		: Allocator(Count * Stride), Stride(Stride)
	{
		NEXUS_ASSERT(Stride >= sizeof(void*), "Element size should be at least : %d bytes", sizeof(void*))

		Head = (uint64*)GetPointer();
		*Head = reinterpret_cast<uint64>(Head) + Stride;
	}

	PoolAllocator::~PoolAllocator()
	{
	}

	void* PoolAllocator::Allocate()
	{
		NEXUS_ASSERT(Head != nullptr, "Head is null");
		NEXUS_ASSERT(*Head != 0, "Next head is null");
		NEXUS_ASSERT(CanAllocate(), "Pool is Full");

		void* Pointer = Head;
		
		Head = reinterpret_cast<uint64*>(*Head);
		if (*Head == 0)
		{
			*Head = reinterpret_cast<uint64>(Head) + Stride;
		}
		
		SetAmount(UsedAmount() + Stride);

		return Pointer;
	}

	void PoolAllocator::Free(void* Pointer)
	{
		NEXUS_ASSERT(Head != nullptr, "Head is null");
		NEXUS_ASSERT(*Head != 0, "Next head is null");
		NEXUS_ASSERT(ValidAddress(Pointer), "Address is outside of the pool");

#if NEXUS_DEBUG
		Memory::MemSet(Pointer, 0, Stride);
#endif

		uint64 Address = reinterpret_cast<uint64>(Head);
		Head = (uint64*)Pointer;
		*Head = Address;

		SetAmount(UsedAmount() - Stride);
	}

	void PoolAllocator::Clear()
	{
		uint64* Pointer = (uint64*)GetPointer();

#if NEXUS_DEBUG
		Memory::MemSet(Pointer, 0, TotalAmount());
#endif

		Head = Pointer;
		*Head = reinterpret_cast<uint64>(Head) + Stride;

		SetAmount(0);
	}

	uint64 PoolAllocator::SlotAvailable() const
	{
		return FreeAmount() / Stride;
	}
	
	bool PoolAllocator::CanAllocate() const
	{
		return SlotAvailable() > 0;
	}

	bool PoolAllocator::ValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null");

		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(GetPointer());
		uint64 End = Start + TotalAmount();

		return Address > Start && Address < End;
	}
}