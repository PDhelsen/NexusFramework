#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Memory/MemoryTracker.h"

namespace NxFr
{
	MemoryTracker::MemoryTracker()
		: Alloctions(97, nullptr), Amount(0), Count(0), Recording(false)
	{
	}

	MemoryTracker::~MemoryTracker()
	{
	}

	void MemoryTracker::RecordAllocation(void* Pointer, uint64 Size)
	{
		if (!Recording)
		{
			return;
		}

		Lock LockGuard(Guard);
		Recording = false;

		NX_ASSERT(Pointer, Default, "Null pointer");
		NX_ASSERT(Amount < Integer::MaxUI64 - Size, Default, "Allocation Amount will overflow");
		NX_ASSERT(Count < Integer::MaxUI64, Default, "Allocation Count will overflow");

		Alloctions.AppendOrAssign(Pointer, Size);
		Amount += Size;
		Count++;

		Recording = true;
	}

	void MemoryTracker::RecordReallocation(void* Pointer, void* Reallocated, uint64 Size)
	{
		RecordDeallocation(Pointer);
		RecordAllocation(Reallocated, Size);
	}

	void MemoryTracker::RecordDeallocation(void* Pointer)
	{
		if (!Recording)
		{
			return;
		}

		if (!Pointer)
		{
			return;
		}

		Lock LockGuard(Guard);
		Recording = false;

		uint64* Allocation = Alloctions.TryGet(Pointer);
		if (!Allocation)
		{
			NX_LOG(Info, Verbose, "Untracked allocation %p", Pointer);
			return;
		}

		uint64 Size = *Allocation;
		NX_ASSERT(Amount >= Size, Default, "Allocation Amount will overflow");
		NX_ASSERT(Count > 0, Default, "Allocation Count will overflow");

		Alloctions.Remove(Pointer);
		Amount -= Size;
		Count--;

		Recording = true;
	}

	void MemoryTracker::StartRecording()
	{
		if (Recording)
		{
			NX_LOG(Warning, Default, "MemoryTracker is already recording");
			return;
		}

		Recording = true;
	}

	void MemoryTracker::StopRecording()
	{
		if (!Recording)
		{
			NX_LOG(Warning, Default, "MemoryTracker is not recording");
			return;
		}

		Recording = false;
	}
}
