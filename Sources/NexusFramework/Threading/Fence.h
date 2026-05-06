#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Threading/Mutex.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	struct Fence
	{
	public:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, Fence)
		NEXUS_FRAMEWORK_API Fence(uint64 Value);
		NEXUS_FRAMEWORK_API ~Fence();

		NEXUS_FRAMEWORK_API void Wait();
		NEXUS_FRAMEWORK_API void Signal();

		NEXUS_FRAMEWORK_API void Set(uint64 Value);
		NEXUS_FRAMEWORK_API uint64 Get();

	private:
		ConditionVariable Condition;
		Mutex Guard;
		uint64 Counter;
	};
}
