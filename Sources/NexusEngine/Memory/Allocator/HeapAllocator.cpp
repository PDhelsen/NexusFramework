#include "Core/NexusEnginePch.h"
#include "HeapAllocator.h"
#include "Memory/Handle/HandleManager.h"

namespace NxEn
{
	HeapAllocator::HeapAllocator(uint64 Size)
		: Allocator(Size)
	{
		Clear();
	}

	HeapAllocator::~HeapAllocator()
	{
	}

	void* HeapAllocator::Allocate(uint64 Size /* 0 */, uint64 Alignement /* 0 */)
	{
		Size = GetAlignedSize(Size);
		HeapSlot* Slot = GetHeapSlot(Size);

		NEXUS_ASSERT(Slot != nullptr, "Failed to find a big enough heap slot")

		uint64 HeapSlotAddress = reinterpret_cast<uint64>(Slot);
		uint64 MemoryAddress = HeapSlotAddress + sizeof(HeapSlot);
		uint64 NextAddress = MemoryAddress + Size;

		bool AddHeapSlot = Slot->Next == nullptr;
		bool InsertHeapSlot = !AddHeapSlot && (reinterpret_cast<uint64>(Slot->Next) - NextAddress >= sizeof(HeapSlot) + NEXUS_MEMORY_ALIGN);

		HeapSlot* NewHeapSlot = nullptr;
		if (AddHeapSlot || InsertHeapSlot)
		{
			NewHeapSlot = reinterpret_cast<HeapSlot*>(NextAddress);
			NewHeapSlot->Next = InsertHeapSlot ? Slot->Next : nullptr;
			NewHeapSlot->Free = true;
		}

		Slot->Next = AddHeapSlot || InsertHeapSlot ? NewHeapSlot : Slot->Next;
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
		uint64 Size = GetHeapSlotSize(Slot);
		Slot->Free = true;

		EraseMemory(Pointer, Size);
		UpdateAmount(Size, false);

		if (Slot->Next != nullptr && Slot->Next->Free)
		{
			HeapSlot* Next = Slot->Next;
			Slot->Next = Next->Next;

			EraseMemory(Next, sizeof(HeapSlot));
			UpdateAmount(sizeof(HeapSlot), false);
		}
	}

	void HeapAllocator::Clear()
	{
		WipeoutMemory();
		ResetAmount();

		Root = (HeapSlot*)GetMemoryBlock();
		Root->Next = nullptr;
		Root->Free = true;

		UpdateAmount(sizeof(HeapSlot), true);
	}

	bool HeapAllocator::CanAllocate(uint64 Size /* 0 */, uint64 Alignement /* 0 */) const
	{
		Size = GetAlignedSize(Size);
		return GetHeapSlot(Size) != nullptr;
	}
	
	bool HeapAllocator::IsValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")
		NEXUS_ASSERT(reinterpret_cast<uint64>(Pointer) % NEXUS_MEMORY_ALIGN == 0, "Pointer is not aligned")

		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(GetMemoryBlock());
		uint64 End = Start + TotalAmount();

		return Address >= Start && Address < End;
	}

	void HeapAllocator::Defragment()
	{
		NEXUS_LOG(Engine, Info, 0, "Starting defragmentation (Current amount : %d)", UsedAmount());

		HeapSlot* Slot = Root;
		while (true)
		{
			// Last slot
			if (Slot->Next == nullptr)
			{
				break;
			}

			// Slot is currently used
			if (!Slot->Free)
			{
				Slot = Slot->Next;
				continue;
			}

			// Slot is free and next one too, so we can merge the two free slot together
			if (Slot->Next->Free)
			{
				HeapSlot* Next = Slot->Next;
				Slot->Next = Next->Next;

				EraseMemory(Next, sizeof(HeapSlot));
				UpdateAmount(sizeof(HeapSlot), false);
			}
			// Slot is free but next one not, so we move the next one into the current one to bubble up the free space at the end of the heap
			else
			{
				// Check if next data is stored in an Handle and so can be moved in memory
				uint8* Data = GetHeapSlotData(Slot->Next);
				Handle<uint8> Handle = HandleManager::GetInstance()->FindHandle<uint8>(Data);
				if (!Handle.IsValid())
				{
					Slot = Slot->Next;
					continue;
				}

				// Get slot info
				uint64 SlotSize = GetHeapSlotSize(Slot);
				uint64 NextSize = GetHeapSlotSize(Slot->Next);
				HeapSlot* NextNext = Slot->Next->Next;

				// Move data 
				Memory::MemCopy(Slot->Next, Slot, sizeof(HeapSlot) + NextSize);
				Data = GetHeapSlotData(Slot);
				HandleManager::GetInstance()->UpdateHandle(Handle, Data);
				NEXUS_LOG(Engine, Info, 0, "Moved from %p to %p", Slot->Next, Slot);

				// Update HeapSlot
				uint64 NewAddress = reinterpret_cast<uint64>(Slot) + sizeof(HeapSlot) + NextSize;
				HeapSlot* NewSlot = reinterpret_cast<HeapSlot*>(NewAddress);
				NewSlot->Next = NextNext;
				NewSlot->Free = true;
				Slot->Next = NewSlot;
				Slot->Free = false;

				Slot = NewSlot;
			}
		}

		NEXUS_LOG(Engine, Info, 0, "End defragmentation (Current amount : %d)", UsedAmount());
	}

	HeapSlot* HeapAllocator::GetHeapSlot(uint64 Size) const
	{
		HeapSlot* Slot = Root;
		while (!Slot->Free || GetHeapSlotSize(Slot) < Size)
		{
			Slot = Slot->Next;

			if (Slot == nullptr)
			{
				return nullptr;
			}
		}

		return Slot;
	}

	uint64 HeapAllocator::GetAlignedSize(uint64 Size) const
	{
		uint64 UnalignedBytes = Size % NEXUS_MEMORY_ALIGN;
		Size += UnalignedBytes == 0 ? 0 : NEXUS_MEMORY_ALIGN - UnalignedBytes;
		return Size;
	}

	uint64 HeapAllocator::GetHeapSlotSize(HeapSlot* Slot) const
	{
		uint64 Size = 0;
		if (Slot->Next != nullptr)
		{
			Size = reinterpret_cast<uint64>(Slot->Next) - reinterpret_cast<uint64>(Slot);
		}
		else
		{
			Size = reinterpret_cast<uint64>(GetMemoryBlock()) + TotalAmount() - reinterpret_cast<uint64>(Slot);
		}

		return Size - sizeof(HeapSlot);
	}

	uint8* HeapAllocator::GetHeapSlotData(HeapSlot* Slot) const
	{
		uint64 Address = reinterpret_cast<uint64>(Slot) + sizeof(HeapSlot);
		return reinterpret_cast<uint8*>(Address);
	}
}