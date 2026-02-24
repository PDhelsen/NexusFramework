#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Threading/Atomic.h"

namespace NxFr
{
	struct Mutex
	{
		friend struct ConditionVariable;

	public:
		NEXUS_FRAMEWORK_API Mutex();
		NEXUS_FRAMEWORK_API ~Mutex();

		NEXUS_FRAMEWORK_API void Lock();
		NEXUS_FRAMEWORK_API void Unlock();

	private:
		void* Handle;
		Atomic Owner;
	};

	struct Lock
	{
	public:
		NEXUS_FRAMEWORK_API Lock(Mutex& Guard);
		NEXUS_FRAMEWORK_API Lock(const Lock& Other) = delete;
		NEXUS_FRAMEWORK_API Lock(Lock&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~Lock();

		NEXUS_FRAMEWORK_API Lock& operator=(const Lock& Other) = delete;
		NEXUS_FRAMEWORK_API Lock& operator=(Lock&& Other) noexcept = delete;

	private:
		Mutex& Guard;
	};
}

