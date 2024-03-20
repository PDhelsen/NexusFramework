#include "Core/NexusEnginePch.h"
#include "HeapAllocator.h"

namespace NxEn
{
#define NEXUS_HEAP_ALIGN 16

	HeapAllocator::HeapAllocator(uint64 Size)
		: Allocator(Size)
	{
		Clear();
	}

	HeapAllocator::~HeapAllocator()
	{
	}

	void* HeapAllocator::Allocate(uint64 Size)
	{
		uint64 UnalignedBytes = Size % NEXUS_HEAP_ALIGN;
		Size += UnalignedBytes == 0 ? 0 : NEXUS_HEAP_ALIGN - UnalignedBytes;
		UpdateAmount(Size, true);

		Size += sizeof(HeapSlot);

		HeapSlot* Slot = GetHeapSlot(Size);
		NEXUS_ASSERT(Slot != nullptr, "Failed to find a big enough heap slot")

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

			UpdateAmount(sizeof(HeapSlot), true);
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

				UpdateAmount(sizeof(HeapSlot), true);
			}
			else
			{
				Slot->Free = false;
			}
		}

		return reinterpret_cast<void*>(PointerAddress);
	}

	void HeapAllocator::Free(void* Pointer)
	{
		NEXUS_ASSERT(IsValidAddress(Pointer), "Address is outside ouf the heap")

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address -= sizeof(HeapSlot);

		HeapSlot* Slot = reinterpret_cast<HeapSlot*>(Address);
		Slot->Free = true;

		EraseMemory(Pointer, Slot->Size - sizeof(HeapSlot));
		UpdateAmount((Slot->Size - sizeof(HeapSlot)), false);

		if (Slot->Next != nullptr && Slot->Next->Free)
		{
			HeapSlot* Next = Slot->Next;
			Slot->Size += Next->Size;
			Slot->Next = Next->Next;

			EraseMemory(Next, sizeof(HeapSlot));
			UpdateAmount(sizeof(HeapSlot), false);
		}

		if (Slot->Previous != nullptr && Slot->Previous->Free)
		{
			HeapSlot* Previous = Slot->Previous;
			Previous->Size += Slot->Size;
			Previous->Next = Slot->Next;

			EraseMemory(Slot, sizeof(HeapSlot));
			UpdateAmount(sizeof(HeapSlot), false);
		}
	}

	void HeapAllocator::Clear()
	{
		WipeoutMemory();
		ResetAmount();

		Root = (HeapSlot*)GetMemoryBlock();
		Root->Previous = nullptr;
		Root->Next = nullptr;
		Root->Size = TotalAmount();
		Root->Free = true;

		UpdateAmount(sizeof(HeapSlot), true);
	}

	bool HeapAllocator::CanAllocate(uint64 Size) const
	{
		uint64 UnalignedBytes = Size % NEXUS_HEAP_ALIGN;
		Size += NEXUS_HEAP_ALIGN - UnalignedBytes;
		Size += sizeof(HeapSlot);

		return GetHeapSlot(Size) != nullptr;
	}
	
	bool HeapAllocator::IsValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")
		NEXUS_ASSERT(reinterpret_cast<uint64>(Pointer) % NEXUS_HEAP_ALIGN == 0, "Pointer is not aligned")

		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(GetMemoryBlock());
		uint64 End = Start + TotalAmount();

		return Address >= Start && Address < End;
	}

	HeapSlot* HeapAllocator::GetHeapSlot(uint64 Size) const
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