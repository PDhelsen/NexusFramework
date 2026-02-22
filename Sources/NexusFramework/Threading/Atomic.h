#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"

namespace NxFr
{
	struct Atomic
	{
	public:
		NEXUS_FRAMEWORK_API Atomic();
		NEXUS_FRAMEWORK_API Atomic(bool Value);
		NEXUS_FRAMEWORK_API Atomic(uint64 Value);
		NEXUS_FRAMEWORK_API ~Atomic();

		NEXUS_FRAMEWORK_API operator uint64();
		NEXUS_FRAMEWORK_API operator bool();

		NEXUS_FRAMEWORK_API uint64 Add(uint64 Target);
		NEXUS_FRAMEWORK_API uint64 Increment();
		NEXUS_FRAMEWORK_API uint64 Decrement();
		NEXUS_FRAMEWORK_API uint64 Load();
		NEXUS_FRAMEWORK_API bool Is();
		NEXUS_FRAMEWORK_API uint64 Store(uint64 Target);
		NEXUS_FRAMEWORK_API bool Store(bool Target);

	private:
		volatile uint64 Value;
	};
}

