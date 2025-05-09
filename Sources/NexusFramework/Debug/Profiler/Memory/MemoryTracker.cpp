#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Memory/MemoryTracker.h"

namespace NxFr
{
	bool Track = true;

	MemoryTracker::UntrackedScope::UntrackedScope()
		: State(Track)
	{
		Track = false;
	}

	MemoryTracker::UntrackedScope::~UntrackedScope()
	{
		Track = State;
	}

	MemoryTracker* MemoryTracker::GetInstance()
	{
		static MemoryTracker* Tracker = Create();
		return Tracker;
	}

	bool MemoryTracker::CanTrack()
	{
		return Track;
	}

	MemoryTracker* MemoryTracker::Create()
	{
		UntrackedScope Untracked;

		MemoryTracker* Instance = new MemoryTracker();
		return Instance;
	}

	void MemoryTracker::Destroy(MemoryTracker* Instance)
	{
		UntrackedScope Untracked;

		delete Instance;
	}

	void MemoryTracker::RecordAllocation(void* Pointer, uint64 Size)
	{
		UntrackedScope Untracked;

		NEXUS_ASSERT(Pointer, Default, "Null pointer");
		NEXUS_ASSERT(Amount < Integer::MaxUI64 - Size, Default, "Allocation Amount will overflow");
		NEXUS_ASSERT(Count < Integer::MaxUI64, Default, "Allocation Count will overflow");

		Alloctions.Append(Pointer, Size);
		Amount += Size;
		Count++;
	}

	void MemoryTracker::RecordReallocation(void* Pointer, void* Reallocated, uint64 Size)
	{
		UntrackedScope Untracked;

		RecordDeallocation(Pointer);
		RecordAllocation(Reallocated, Size);
	}

	void MemoryTracker::RecordDeallocation(void* Pointer)
	{
		UntrackedScope Untracked;

		if (!Pointer)
		{
			return;
		}

		uint64* Allocation = Alloctions.TryGet(Pointer);
		if (!Allocation)
		{
			NEXUS_LOG(Warning, Verbose, "Failed to find allocation %p", Pointer);
			return;
		}

		uint64 Size = *Allocation;
		NEXUS_ASSERT(Amount >= Size, Default, "Allocation Amount will overflow");
		NEXUS_ASSERT(Count > 0, Default, "Allocation Count will overflow");

		Alloctions.Remove(Pointer);
		Amount -= Size;
		Count--;
	}

	MemoryTracker::MemoryTracker()
		: Alloctions(Dictionary<void*, uint64>::DefaultSize, nullptr), Amount(0), Count(0)
	{
	}

	MemoryTracker::~MemoryTracker()
	{
	}
}
