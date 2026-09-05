#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/ManagedAllocator.h"

#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Time/Stopwatch.h"

namespace NxFr
{
	ManagedAllocator::Heap::Heap(uint64 Size)
		: HeapAllocator(Size)
	{
	}

	ManagedAllocator::Heap::~Heap()
	{
	}

	void ManagedAllocator::Heap::Defragment(const HandleMemroyInfos& Infos, float& Time, uint64& Count)
	{
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
				Amount -= sizeof(HeapSlot);
			}
			// Slot is free but next one not, so we move the next one into the current one to bubble up the free space at the end of the heap
			else
			{
				// Check if next data is stored in an Handle and so can be moved in memory
				void* Data = GetHeapSlotMemory(Slot->Next);
				const HandleManagerInfo* Bucket = Infos.TryGet(Data);
				if (Bucket == nullptr)
				{
					Slot = Slot->Next;
					continue;
				}

				// Get slot info
				HandleManagerInfo Info = *Bucket;
				uint64 SlotSize = GetHeapSlotSize(Slot);
				uint64 NextSize = GetHeapSlotSize(Slot->Next);
				HeapSlot* NextNext = Slot->Next->Next;

				// Move data
				Memory::MemMove(Slot->Next, Slot, sizeof(HeapSlot) + NextSize);
				Data = GetHeapSlotMemory(Slot);
				Info.GetSecond()->Update(Info.GetFirst(), Data);

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
	}

	ManagedAllocator::ManagedAllocator(uint64 Size, uint64 Count)
		: ContinuousAllocator(Size, ContinuousAllocator::DefaultCreator<Heap>()), Manager(Count)
	{
	}

	ManagedAllocator::~ManagedAllocator()
	{
	}

	void ManagedAllocator::Defragment()
	{
		float Time = 0.0f;
		uint64 Count = 0;
		Defragment(Time, Count);
	}

	void ManagedAllocator::Defragment(float Time)
	{
		uint64 Count = 0;
		Defragment(Time, Count);
	}

	void ManagedAllocator::Defragment(uint64 Count)
	{
		float Time = 0.0f;
		Defragment(Time, Count);
	}

	void ManagedAllocator::Defragment(float& Time, uint64& Count)
	{
		Infos.Clear();
		Manager.GetInfos(Infos);

		for (auto* Alloc : Allocators)
		{
			static_cast<Heap*>(Alloc)->Defragment(Infos, Time, Count);
		}
	}
}
