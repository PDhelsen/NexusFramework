#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "HeapAllocator.h"

#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"

#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Time/Stopwatch.h"

namespace NxFr
{
	HeapAllocator::HeapAllocator(uint64 Size)
		: MemoryAllocator(Size), Root(nullptr), Cache(nullptr)
	{
		Clear();
	}

	HeapAllocator::~HeapAllocator()
	{
	}

	void HeapAllocator::Clear()
	{
		WipeoutMemory();
		ResetAmount();

		Reset();
	}

	bool HeapAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		Size = GetAlignedSize(Size);
		Cache = GetHeapSlot(Size);
		return Cache != nullptr;
	}

	bool HeapAllocator::BelongToAllocator(void* Pointer) const
	{
		return Pointer && IsPointerInMemoryBlock(Pointer);
	}

	void HeapAllocator::Defragment(HandleManager* Manager)
	{
		Defragment(Manager, 0, 0);
	}

	void HeapAllocator::Defragment(HandleManager* Manager, float Time)
	{
		Defragment(Manager, Time, 0);
	}

	void HeapAllocator::Defragment(HandleManager* Manager, uint64 Count)
	{
		Defragment(Manager, 0, Count);
	}

	void* HeapAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		Size = GetAlignedSize(Size);
		HeapSlot* Slot = GetHeapSlot(Size);

		NEXUS_ASSERT(Slot, Default, "Allocator is full");

		void* Pointer = GetHeapSlotMemory(Slot);

		UpdateHeapSlot(Slot, Size);

		IncreaseAmount(Size);
		Cache = nullptr;
		return Pointer;
	}

	void* HeapAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (!Pointer)
		{
			return nullptr;
		}

		NEXUS_ASSERT(IsPointerInMemoryBlock(Pointer), Default, "Invalid pointer");

		void* NewPointer = nullptr;
		Size = GetAlignedSize(Size);

		HeapSlot* Slot = GetHeapSlot(Pointer);
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
				IncreaseAmount(Size - CurrentSize);
			}
			else
			{
				DecreaseAmount(CurrentSize - Size);
			}

			NewPointer = GetHeapSlotMemory(Slot);
		}
		else
		{
			NewPointer = Allocate(Size, Alignement);
			Memory::MemMove(Pointer, NewPointer, Size);
			Free(Pointer);
		}

		Cache = Slot;
		return NewPointer;
	}

	void HeapAllocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		NEXUS_ASSERT(IsPointerInMemoryBlock(Pointer), Default, "Invalid pointer");

		HeapSlot* Slot = GetHeapSlot(Pointer);
		uint64 Size = GetHeapSlotSize(Slot);
		Slot->Free = true;

		EraseMemory(Pointer, Size);

		if (Slot->Next != nullptr && Slot->Next->Free)
		{
			RemoveNextHeapSlot(Slot);
		}

		DecreaseAmount(Size);
		Cache = Slot;
	}

	void HeapAllocator::UpdateHeapSlot(HeapSlot* Slot, uint64 Size)
	{
		uint64 HeapSlotAddress = reinterpret_cast<uint64>(Slot);
		uint64 MemoryAddress = HeapSlotAddress + sizeof(HeapSlot);
		uint64 NextAddress = MemoryAddress + Size;
		void* NextPointer = reinterpret_cast<void*>(NextAddress);
		bool NextIsInsideHeap = NextPointer && IsPointerInMemoryBlock(NextPointer);

		NEXUS_ASSERT(Slot->Next == nullptr || reinterpret_cast<uint64>(Slot->Next) >= NextAddress, Default, "Overlap");

		bool AddHeapSlot = NextIsInsideHeap && Slot->Next == nullptr;
		bool InsertHeapSlot = NextIsInsideHeap && !AddHeapSlot && (reinterpret_cast<uint64>(Slot->Next) - NextAddress >= sizeof(HeapSlot) + Memory::DefaultAlignement);

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
			IncreaseAmount(sizeof(HeapSlot));
		}
	}

	void HeapAllocator::RemoveNextHeapSlot(HeapSlot* Slot)
	{
		HeapSlot* Next = Slot->Next;
		Slot->Next = Next->Next;

		EraseMemory(Next, sizeof(HeapSlot));
		DecreaseAmount(sizeof(HeapSlot));
	}

	HeapAllocator::HeapSlot* HeapAllocator::GetHeapSlot(void* Pointer) const
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address -= sizeof(HeapSlot);
		return reinterpret_cast<HeapSlot*>(Address);
	}

	HeapAllocator::HeapSlot* HeapAllocator::GetHeapSlot(uint64 Size) const
	{
		HeapSlot* Slot = nullptr;

		if (Cache && Cache->Free && GetHeapSlotSize(Cache) >= Size)
		{
			Slot = Cache;
		}
		else
		{
			Slot = Root;
			while (Slot && (!Slot->Free || GetHeapSlotSize(Slot) < Size))
			{
				Slot = Slot->Next;
			}
		}

		return Slot;
	}

	void* HeapAllocator::GetHeapSlotMemory(HeapSlot* Slot) const
	{
		uint64 Address = reinterpret_cast<uint64>(Slot) + sizeof(HeapSlot);
		return reinterpret_cast<void*>(Address);
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

	uint64 HeapAllocator::GetAlignedSize(uint64 Size) const
	{
		uint64 UnalignedBytes = Size % Memory::DefaultAlignement;
		Size += UnalignedBytes == 0 ? 0 : Memory::DefaultAlignement - UnalignedBytes;
		return Size;
	}

	void HeapAllocator::Reset()
	{
		Root = (HeapSlot*)GetMemoryBlock();
		Root->Next = nullptr;
		Root->Free = true;

		Cache = nullptr;

		IncreaseAmount(sizeof(HeapSlot));
	}

	void HeapAllocator::Defragment(HandleManager* Manager, float Time, uint64 Count)
	{
		NEXUS_LOG(Info, Verbose, "Starting defragmentation (Current amount : %d)", UsedAmount());

		Dictionary<void*, Handle<void>> Handles = Manager->GetHandlesPointingToMemoryRange(GetMemoryBlock(), TotalAmount());

		bool All = Time <= 0.0f && Count == 0;
		Stopwatch Watch = Stopwatch(true);
		uint64 Iteration = 0;

		HeapSlot* Slot = Root;
		while (All || (!All && Time > 0.0f) || (!All && Count > 0))
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
				DecreaseAmount(sizeof(HeapSlot));
			}
			// Slot is free but next one not, so we move the next one into the current one to bubble up the free space at the end of the heap
			else
			{
				// Check if next data is stored in an Handle and so can be moved in memory
				void* Data = GetHeapSlotMemory(Slot->Next);
				auto It = Handles.FindKey(Data);
				if (It == Handles.End())
				{
					Slot = Slot->Next;
					continue;
				}

				// Get slot info
				Handle<void>& Handle = It->Value;
				uint64 SlotSize = GetHeapSlotSize(Slot);
				uint64 NextSize = GetHeapSlotSize(Slot->Next);
				HeapSlot* NextNext = Slot->Next->Next;

				// Move data
				Memory::MemMove(Slot->Next, Slot, sizeof(HeapSlot) + NextSize);
				Data = GetHeapSlotMemory(Slot);
				Manager->UpdateHandle(Handle, Data);
				NEXUS_LOG(Info, Verbose, "Moved from %p to %p", Slot->Next, Slot);

				// Update HeapSlot
				uint64 NewAddress = reinterpret_cast<uint64>(Slot) + sizeof(HeapSlot) + NextSize;
				HeapSlot* NewSlot = reinterpret_cast<HeapSlot*>(NewAddress);
				NewSlot->Next = NextNext;
				NewSlot->Free = true;
				Slot->Next = NewSlot;
				Slot->Free = false;

				Slot = NewSlot;
			}

			Count = Count > 0 ? Count - 1 : 0;
			Time = Time > 0.0f ? Time - (float)Watch.Peek(Time::SecondToMilli) : 0.0f;

			Iteration++;
		}

		Watch.Stop();

		NEXUS_LOG(Info, Verbose, "End defragmentation (Current amount : %d)", UsedAmount());
		NEXUS_LOG(Info, Verbose, "Updated %d HeapSlot in %f ms", Iteration, Watch.GetElapsedTime(Time::SecondToMilli));
	}
}
