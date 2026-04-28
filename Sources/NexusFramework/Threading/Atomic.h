#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	struct Atomic
	{
	public:
		NEXUS_FRAMEWORK_API Atomic();
		NEXUS_FRAMEWORK_API Atomic(int64 Value);
		NEXUS_FRAMEWORK_API Atomic(const Atomic& Other) = delete;
		NEXUS_FRAMEWORK_API Atomic(Atomic&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~Atomic();

		NEXUS_FRAMEWORK_API Atomic& operator=(const Atomic&) = delete;
		NEXUS_FRAMEWORK_API Atomic& operator=(Atomic&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API int64 Add(int64 Target);
		NEXUS_FRAMEWORK_API int64 Increment();
		NEXUS_FRAMEWORK_API int64 Decrement();
		NEXUS_FRAMEWORK_API int64 Load() const;
		NEXUS_FRAMEWORK_API void Store(int64 Target);
		NEXUS_FRAMEWORK_API bool CompareExchange(int64 Target, int64 Expected);

	private:
		alignas(8) mutable int64 Value;
	};
}

