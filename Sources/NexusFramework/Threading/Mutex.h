#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"

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
	};

	struct Lock
	{
	public:
		NEXUS_FRAMEWORK_API Lock(Mutex& Target);
		NEXUS_FRAMEWORK_API ~Lock();

	private:
		Mutex& Target;
	};
}

