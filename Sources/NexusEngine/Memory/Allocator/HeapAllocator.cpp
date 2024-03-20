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
		Size = GetAlignedSize(Size);
		HeapSlot* Slot = GetHeapSlot(Size);

		NEXUS_ASSERT(Slot != nullptr, "Failed to find a big enough heap slot")

		uint64 HeapSlotAddress = reinterpret_cast<uint64>(Slot);
		uint64 MemoryAddress = HeapSlotAddress + sizeof(HeapSlot);
		uint64 NextAddress = MemoryAddress + Size;

		bool AddHeapSlot = Slot->Next == nullptr;
		bool InsertHeapSlot = !AddHeapSlot && (reinterpret_cast<uint64>(Slot->Next) - NextAddress > sizeof(HeapSlot) + NEXUS_HEAP_ALIGN);

		HeapSlot* NewHeapSlot = nullptr;
		if (AddHeapSlot || InsertHeapSlot)
		{
			NewHeapSlot = reinterpret_cast<HeapSlot*>(NextAddress);
			NewHeapSlot->Previous = Slot;
			NewHeapSlot->Next = InsertHeapSlot ? Slot->Next : nullptr;
			NewHeapSlot->Size = Slot->Size - Size - sizeof(HeapSlot);
			NewHeapSlot->Free = true;
		}

		Slot->Next = AddHeapSlot || InsertHeapSlot ? NewHeapSlot : Slot->Next;
		Slot->Size = Size;
		Slot->Free = false;

		UpdateAmount(Size, true);
		if (AddHeapSlot || InsertHeapSlot)
		{
			UpdateAmount(sizeof(HeapSlot), true);
		}

		return reinterpret_cast<void*>(MemoryAddress);
	}

	void HeapAllocator::Free(void* Pointer)
	{
		NEXUS_ASSERT(IsValidAddress(Pointer), "Address is outside ouf the heap")

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address -= sizeof(HeapSlot);

		HeapSlot* Slot = reinterpret_cast<HeapSlot*>(Address);
		Slot->Free = true;

		EraseMemory(Pointer, Slot->Size);
		UpdateAmount(Slot->Size, false);

		if (Slot->Next != nullptr && Slot->Next->Free)
		{
			HeapSlot* Next = Slot->Next;
			Slot->Size += Next->Size + sizeof(HeapSlot);
			Slot->Next = Next->Next;

			EraseMemory(Next, sizeof(HeapSlot));
			UpdateAmount(sizeof(HeapSlot), false);
		}

		if (Slot->Previous != nullptr && Slot->Previous->Free)
		{
			HeapSlot* Previous = Slot->Previous;
			Previous->Size += Slot->Size + sizeof(HeapSlot);
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
		Root->Size = TotalAmount() - sizeof(HeapSlot);
		Root->Free = true;

		UpdateAmount(sizeof(HeapSlot), true);
	}

	bool HeapAllocator::CanAllocate(uint64 Size) const
	{
		Size = GetAlignedSize(Size);
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

	uint64 HeapAllocator::GetAlignedSize(uint64 Size) const
	{
		uint64 UnalignedBytes = Size % NEXUS_HEAP_ALIGN;
		Size += UnalignedBytes == 0 ? 0 : NEXUS_HEAP_ALIGN - UnalignedBytes;
		return Size;
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