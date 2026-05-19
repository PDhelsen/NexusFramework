#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Threading/Mutex.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	struct NX_FRAMEWORK_API Fence
	{
	public:
		NX_NOCOPY_NOMOVE(Fence)
		Fence(uint64 Value);
		~Fence();

		void Wait();
		void Signal();

		void Set(uint64 Value);
		uint64 Get();

	private:
		ConditionVariable Condition;
		Mutex Guard;
		uint64 Counter;
	};
}
