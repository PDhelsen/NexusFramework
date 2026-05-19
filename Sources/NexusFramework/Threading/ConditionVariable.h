#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	struct NX_FRAMEWORK_API ConditionVariable
	{
	public:
		NX_NOCOPY_NOMOVE(ConditionVariable)
		ConditionVariable();
		~ConditionVariable();

		void Wait(Mutex& Guard, const Delegate<bool()>& Predicate);
		void Wait(Mutex& Guard);
		void Signal();
		void Broadcast();

	private:
		void* Handle;
	};
}
