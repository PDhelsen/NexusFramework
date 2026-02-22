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
		mutable Atomic State;
		void* Handle;
	};

	struct Lock
	{
	public:
		NEXUS_FRAMEWORK_API Lock(Mutex& Guard);
		NEXUS_FRAMEWORK_API ~Lock();

	private:
		Mutex& Guard;
	};
}

