#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Threading/Atomic.h"

namespace NxFr
{
	struct NEXUS_FRAMEWORK_API Mutex
	{
		friend struct ConditionVariable;

	public:
		NEXUS_NOCOPY_NOMOVE(Mutex)
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();

	private:
		void* Handle;
		Atomic Owner;
	};

	struct NEXUS_FRAMEWORK_API Lock
	{
	public:
		NEXUS_NOCOPY_NOMOVE(Lock)
		Lock(Mutex& Guard);
		~Lock();

	private:
		Mutex& Guard;
	};
}

