#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API MemoryTracker
	{
	public:
		NEXUS_NOCOPY_NOMOVE(MemoryTracker)
		MemoryTracker();
		~MemoryTracker();

		void RecordAllocation(void* Pointer, uint64 Size);
		void RecordReallocation(void* Pointer, void* Reallocated, uint64 Size);
		void RecordDeallocation(void* Pointer);

		void StartRecording();
		void StopRecording();

		uint64 GetAllocatedAmount() const { return Amount; };
		uint64 GetAllocationCount() const { return Count; };

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

	#define NEXUS_TRACKMEMORY_ALLOCATION(Pointer, Size) NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(::NxFr::Globals::Debug::Memory, Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION(Pointer, Reallocated, Size) NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(::NxFr::Globals::Debug::Memory, Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION(Pointer) NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(::NxFr::Globals::Debug::Memory, Pointer)
#else
	#define NEXUS_TRACKMEMORY_ALLOCATION_INSTANCE(Instance, Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION_INSTANCE(Instance, Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION_INSTANCE(Instance, Pointer)
	
	#define NEXUS_TRACKMEMORY_ALLOCATION(Pointer, Size)
	#define NEXUS_TRACKMEMORY_REALLOCATION(Pointer, Reallocated, Size)
	#define NEXUS_TRACKMEMORY_DEALLOCATION(Pointer)
#endif
