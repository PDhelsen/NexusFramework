#include "Core/NexusEnginePch.h"
#include "HeapAllocator.h"

namespace NxEn
{
#define NEXUS_HEAP_ALIGN 16

	HeapAllocator::HeapAllocator(uint64 Size)
		: Allocator(Size)
	{
		Root = (HeapSlot*)GetPointer();
		Root->Previous = nullptr;
		Root->Next = nullptr;
		Root->Size = Size;
		Root->Free = true;

		SetAmount(sizeof(HeapSlot));
	}

	HeapAllocator::~HeapAllocator()
	{
	}
	
	void* HeapAllocator::Allocate(uint64 Size)
	{
		uint64 UnalignedBytes = Size % NEXUS_HEAP_ALIGN;
		Size += UnalignedBytes == 0 ? 0 : NEXUS_HEAP_ALIGN - UnalignedBytes;
		SetAmount(UsedAmount() + Size);

		Size += sizeof(HeapSlot);

		HeapSlot* Slot = GetHeapSlot(Size);
		NEXUS_ASSERT(Slot != nullptr, "Failed to find a big enough heap slot");

		uint64 HeapSlotAddress = reinterpret_cast<uint64>(Slot);
		uint64 PointerAddress = HeapSlotAddress + sizeof(HeapSlot);
		uint64 NextAddress = HeapSlotAddress + Size;

		if (Slot->Next == nullptr)
		{
			HeapSlot* NewSlot = reinterpret_cast<HeapSlot*>(NextAddress);
			NewSlot->Previous = Slot;
			NewSlot->Next = nullptr;
			NewSlot->Size = Slot->Size - Size;
			NewSlot->Free = true;
		
			Slot->Next = NewSlot;
			Slot->Size = Size;
			Slot->Free = false;

			SetAmount(UsedAmount() + sizeof(HeapSlot));
		}
		else
		{
			uint64 NextSlotAddress = reinterpret_cast<uint64>(Slot->Next);
			uint64 FreeSpace = NextSlotAddress - NextAddress;

			if (FreeSpace > sizeof(HeapSlot) + NEXUS_HEAP_ALIGN)
			{
				HeapSlot* NewSlot = reinterpret_cast<HeapSlot*>(NextAddress);
				NewSlot->Previous = Slot;
				NewSlot->Next = Slot->Next;
				NewSlot->Size = Slot->Size - Size;
				NewSlot->Free = true;

				Slot->Next = NewSlot;
				Slot->Size = Size;
				Slot->Free = false;

				SetAmount(UsedAmount() + sizeof(HeapSlot));
			}
			else
			{
				Slot->Free = false;
			}
		}

		// Since the memory pointer is aligned on 16 bytes and the HeapSlot is 32 bytes long,
		// Any address just after the heapslot is aligned on 16 bytes (32 = 16 * 2)
		// It should look like :
		// Root (16 aligned) + Heap slot (16 aligned) + Memory (Forced 16 aligned) + Heap slot (16 aligned) + Memory (Forced 16 aligned) + ... 
		return reinterpret_cast<void*>(PointerAddress);
	}

	void HeapAllocator::Free(void* Pointer)
	{
		NEXUS_ASSERT(ValidAddress(Pointer), "Address is outside ouf the heap");

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address -= sizeof(HeapSlot);

		HeapSlot* Slot = reinterpret_cast<HeapSlot*>(Address);
		Slot->Free = true;

		SetAmount(UsedAmount() - (Slot->Size - sizeof(HeapSlot)));

#if NEXUS_DEBUG
		Memory::MemSet(Pointer, 0, Slot->Size - sizeof(HeapSlot));
#endif

		if (Slot->Next != nullptr && Slot->Next->Free)
		{
			HeapSlot* Next = Slot->Next;
			Slot->Size += Next->Size;
			Slot->Next = Next->Next;

			SetAmount(UsedAmount() - sizeof(HeapSlot));

#if NEXUS_DEBUG
			Memory::MemSet(Next, 0, sizeof(HeapSlot));
#endif
		}

		if (Slot->Previous != nullptr && Slot->Previous->Free)
		{
			HeapSlot* Previous = Slot->Previous;
			Previous->Size += Slot->Size;
			Previous->Next = Slot->Next;

			SetAmount(UsedAmount() - sizeof(HeapSlot));

#if NEXUS_DEBUG
			Memory::MemSet(Slot, 0, sizeof(HeapSlot));
#endif
		}
	}

	void HeapAllocator::Clear()
	{
#if NEXUS_DEBUG
		Memory::MemSet(Root, 0, TotalAmount());
#endif

		Root = (HeapSlot*)GetPointer();
		Root->Previous = nullptr;
		Root->Next = nullptr;
		Root->Size = TotalAmount();
		Root->Free = true;

		SetAmount(sizeof(HeapSlot));
	}

	bool HeapAllocator::CanAllocate(uint64 Size) const
	{
		uint64 UnalignedBytes = Size % NEXUS_HEAP_ALIGN;
		Size += NEXUS_HEAP_ALIGN - UnalignedBytes;
		Size += sizeof(HeapSlot);

		return GetHeapSlot(Size) != nullptr;
	}
	
	bool HeapAllocator::ValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null");
		NEXUS_ASSERT(reinterpret_cast<uint64>(Pointer) % NEXUS_HEAP_ALIGN == 0, "Pointer is not aligned");

		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(GetPointer());
		uint64 End = Start + TotalAmount();

		return Address >= Start && Address < End;
	}

	HeapAllocator::HeapSlot* HeapAllocator::GetHeapSlot(uint64 Size) const
	{
		HeapSlot* Slot = Root;
		while (!Slot->Free || Slot->Size < Size)
		{
			Slot = Slot->Next;

			if (Slot == nullptr)
			{
				return nullptr;
			}
		}

		return Slot;
	}
}