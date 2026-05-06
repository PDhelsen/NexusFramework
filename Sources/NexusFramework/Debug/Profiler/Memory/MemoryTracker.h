#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	class MemoryTracker
	{
	public:
		struct UntrackedScope
		{
		public:
			NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, UntrackedScope)
			NEXUS_FRAMEWORK_API UntrackedScope();
			NEXUS_FRAMEWORK_API ~UntrackedScope();

		private:
			bool State;
		};

		NEXUS_FRAMEWORK_API static MemoryTracker* GetInstance();
		NEXUS_FRAMEWORK_API static bool CanTrack();

		NEXUS_FRAMEWORK_API static MemoryTracker* Create();
		NEXUS_FRAMEWORK_API static void Destroy(MemoryTracker* Instance);

		NEXUS_FRAMEWORK_API void RecordAllocation(void* Pointer, uint64 Size);
		NEXUS_FRAMEWORK_API void RecordReallocation(void* Pointer, void* Reallocated, uint64 Size);
		NEXUS_FRAMEWORK_API void RecordDeallocation(void* Pointer);

		NEXUS_FRAMEWORK_API uint64 GetAllocatedAmount() const { return Amount; };
		NEXUS_FRAMEWORK_API uint64 GetAllocationCount() const { return Count; };

	private:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, MemoryTracker)
		NEXUS_FRAMEWORK_API MemoryTracker();
		NEXUS_FRAMEWORK_API ~MemoryTracker();

	private:
		Dictionary<void*, uint64> Alloctions;
		uint64 Amount;
		uint64 Count;

		Mutex Guard;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(Instance, Pointer, Size) if (MemoryTracker::CanTrack() && Instance) { Instance->RecordAllocation(Pointer, Size); }
	#define NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(Instance, Pointer, Reallocated, Size) if (MemoryTracker::CanTrack() && Instance) { Instance->RecordReallocation(Pointer, Reallocated, Size); }
	#define NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(Instance, Pointer) if (MemoryTracker::CanTrack() && Instance) { Instance->RecordDeallocation(Pointer); }

	#define NEXUS_TRACKMEMORY_ALLOCATION(Pointer, Size) NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(::NxFr::MemoryTracker::GetInstance(), Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION(Pointer, Reallocated, Size) NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(::NxFr::MemoryTracker::GetInstance(), Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION(Pointer) NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(::NxFr::MemoryTracker::GetInstance(), Pointer)
#else
	#define NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(Instance, Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(Instance, Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(Instance, Pointer)
	
	#define NEXUS_TRACKMEMORY_ALLOCATION(Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION(Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION(Pointer)
#endif
