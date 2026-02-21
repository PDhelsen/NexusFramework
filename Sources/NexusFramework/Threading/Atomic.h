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

		operator uint64();
		operator bool();

		NEXUS_FRAMEWORK_API void Add(uint64 Target);
		NEXUS_FRAMEWORK_API void Increment();
		NEXUS_FRAMEWORK_API void Decrement();
		NEXUS_FRAMEWORK_API uint64 Load();
		NEXUS_FRAMEWORK_API bool Is();
		NEXUS_FRAMEWORK_API void Store(uint64 Target);
		NEXUS_FRAMEWORK_API void Store(bool Target);

	private:
		volatile uint64 Value;
	};
}

