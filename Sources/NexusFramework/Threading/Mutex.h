#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Threading/Atomic.h"

namespace NxFr
{
	struct NX_FRAMEWORK_API Mutex
	{
		friend struct ConditionVariable;

	public:
		NX_NOCOPY_NOMOVE(Mutex)
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();

	private:
		void* Handle;
		Atomic Owner;
	};

	struct NX_FRAMEWORK_API Lock
	{
	public:
		NX_NOCOPY_NOMOVE(Lock)
		Lock(Mutex& Guard);
		~Lock();

	private:
		Mutex& Guard;
	};
}

