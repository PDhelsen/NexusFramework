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
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, MemoryTracker)
		NEXUS_FRAMEWORK_API MemoryTracker();
		NEXUS_FRAMEWORK_API ~MemoryTracker();

		NEXUS_FRAMEWORK_API void RecordAllocation(void* Pointer, uint64 Size);
		NEXUS_FRAMEWORK_API void RecordReallocation(void* Pointer, void* Reallocated, uint64 Size);
		NEXUS_FRAMEWORK_API void RecordDeallocation(void* Pointer);

		NEXUS_FRAMEWORK_API void StartRecording();
		NEXUS_FRAMEWORK_API void StopRecording();

		NEXUS_FRAMEWORK_API uint64 GetAllocatedAmount() const { return Amount; };
		NEXUS_FRAMEWORK_API uint64 GetAllocationCount() const { return Count; };

	private:
		Dictionary<void*, uint64> Alloctions;
		uint64 Amount;
		uint64 Count;
		bool Recording;

		Mutex Guard;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(Instance, Pointer, Size) if (Instance) { Instance->RecordAllocation(Pointer, Size); }
	#define NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(Instance, Pointer, Reallocated, Size) if (Instance) { Instance->RecordReallocation(Pointer, Reallocated, Size); }
	#define NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(Instance, Pointer) if (Instance) { Instance->RecordDeallocation(Pointer); }

	#define NEXUS_TRACKMEMORY_ALLOCATION(Pointer, Size) NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(::NxFr::Globals::Memory, Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION(Pointer, Reallocated, Size) NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(::NxFr::Globals::Memory, Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION(Pointer) NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(::NxFr::Globals::Memory, Pointer)
#else
	#define NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(Instance, Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(Instance, Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(Instance, Pointer)
	
	#define NEXUS_TRACKMEMORY_ALLOCATION(Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION(Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION(Pointer)
#endif
