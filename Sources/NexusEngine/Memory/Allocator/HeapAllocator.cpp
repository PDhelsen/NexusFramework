#include "Core/NexusEnginePch.h"
#include "HeapAllocator.h"

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

	void* HeapAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		Size = GetAlignedSize(Size);
		HeapSlot* Slot = GetHeapSlot(Size);

		if (Slot == nullptr)
		{
			return nullptr;
		}

		UpdateHeapSlot(Slot, Size);

		UpdateAmount(Size, true);

		return GetHeapSlotMemory(Slot);
	}

	void* HeapAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (!IsAllocatedAddress(Pointer))
		{
			return nullptr;
		}

		void* NewPointer = nullptr;
		Size = GetAlignedSize(Size);

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address -= sizeof(HeapSlot);

		HeapSlot* Slot = reinterpret_cast<HeapSlot*>(Address);
		uint64 CurrentSize = GetHeapSlotSize(Slot);
		bool Shrink = CurrentSize >= Size;

		if ((CurrentSize + GetHeapSlotSize(Slot->Next) >= Size && Slot->Next->Free) || Shrink)
		{
			if (!Shrink)
			{
				RemoveNextHeapSlot(Slot);
			}

			UpdateHeapSlot(Slot, Size);

			if (!Shrink)
			{
				UpdateAmount(Size - CurrentSize, true);
			}
			else
			{
				UpdateAmount(CurrentSize - Size, false);
			}

			NewPointer = GetHeapSlotMemory(Slot);
		}
		else
		{
			NewPointer = Allocate(Size, Alignement);
			Memory::MemMove(Pointer, NewPointer, Size);
			Free(Pointer);
		}

		return NewPointer;
	}

	void HeapAllocator::Free(void* Pointer)
	{
		if (!IsAllocatedAddress(Pointer))
		{
			return;
		}

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address -= sizeof(HeapSlot);

		HeapSlot* Slot = reinterpret_cast<HeapSlot*>(Address);
		uint64 Size = GetHeapSlotSize(Slot);
		Slot->Free = true;

		EraseMemory(Pointer, Size);
		UpdateAmount(Size, false);

		if (Slot->Next != nullptr && Slot->Next->Free)
		{
			RemoveNextHeapSlot(Slot);
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

	bool HeapAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		Size = GetAlignedSize(Size);
		return GetHeapSlot(Size) != nullptr;
	}
	
	bool HeapAllocator::IsAllocatedAddress(void* Pointer) const
	{
		return Pointer != nullptr && IsPointerInside(Pointer);
	}

	// TODO: Implementation - Memory Defragmentation - Defragment heap over multiple frame
	// TODO: Optimization - Algo - Defragment memory
	void HeapAllocator::Defragment()
	{
		NEXUS_LOG(Engine, Info, "Routine", "Starting defragmentation (Current amount : %d)", UsedAmount());

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
				uint8* Data = reinterpret_cast<uint8*>(GetHeapSlotMemory(Slot->Next));
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
				Memory::MemMove(Slot->Next, Slot, sizeof(HeapSlot) + NextSize);
				Data = reinterpret_cast<uint8*>(GetHeapSlotMemory(Slot));
				HandleManager::GetInstance()->UpdateHandle(Handle, Data);
				NEXUS_LOG(Engine, Info, "Routine", "Moved from %p to %p", Slot->Next, Slot);

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

		NEXUS_LOG(Engine, Info, "Routine", "End defragmentation (Current amount : %d)", UsedAmount());
	}

	void HeapAllocator::UpdateHeapSlot(HeapSlot* Slot, uint64 Size)
	{
		uint64 HeapSlotAddress = reinterpret_cast<uint64>(Slot);
		uint64 MemoryAddress = HeapSlotAddress + sizeof(HeapSlot);
		uint64 NextAddress = MemoryAddress + Size;
		bool NextIsInsideHeap = IsAllocatedAddress(reinterpret_cast<void*>(NextAddress));

		NEXUS_ASSERT(Slot->Next == nullptr || reinterpret_cast<uint64>(Slot->Next) >= NextAddress, "Overlap");

		bool AddHeapSlot = NextIsInsideHeap && Slot->Next == nullptr;
		bool InsertHeapSlot = NextIsInsideHeap && !AddHeapSlot && (reinterpret_cast<uint64>(Slot->Next) - NextAddress >= sizeof(HeapSlot) + NEXUS_MEMORY_ALIGN);

		HeapSlot* NewHeapSlot = nullptr;
		if (AddHeapSlot || InsertHeapSlot)
		{
			NewHeapSlot = reinterpret_cast<HeapSlot*>(NextAddress);
			NewHeapSlot->Next = InsertHeapSlot ? Slot->Next : nullptr;
			NewHeapSlot->Free = true;
		}

		Slot->Next = !NextIsInsideHeap ? nullptr : AddHeapSlot || InsertHeapSlot ? NewHeapSlot : Slot->Next;
		Slot->Free = false;

		if (AddHeapSlot || InsertHeapSlot)
		{
			UpdateAmount(sizeof(HeapSlot), true);
		}
	}

	void HeapAllocator::RemoveNextHeapSlot(HeapSlot* Slot)
	{
		HeapSlot* Next = Slot->Next;
		Slot->Next = Next->Next;

		EraseMemory(Next, sizeof(HeapSlot));
		UpdateAmount(sizeof(HeapSlot), false);
	}

	HeapAllocator::HeapSlot* HeapAllocator::GetHeapSlot(uint64 Size) const
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

	void* HeapAllocator::GetHeapSlotMemory(HeapSlot* Slot) const
	{
		uint64 Address = reinterpret_cast<uint64>(Slot) + sizeof(HeapSlot);
		return reinterpret_cast<void*>(Address);
	}
}
