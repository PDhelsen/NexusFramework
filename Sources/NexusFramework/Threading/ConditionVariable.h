#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	struct ConditionVariable
	{
	public:
		NEXUS_FRAMEWORK_API ConditionVariable();
		NEXUS_FRAMEWORK_API ConditionVariable(const ConditionVariable& Other) = delete;
		NEXUS_FRAMEWORK_API ConditionVariable(ConditionVariable&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~ConditionVariable();

		NEXUS_FRAMEWORK_API ConditionVariable& operator=(const ConditionVariable& Other) = delete;
		NEXUS_FRAMEWORK_API ConditionVariable& operator=(ConditionVariable&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Wait(Mutex& Guard, const Delegate<bool()>& Predicate);
		NEXUS_FRAMEWORK_API void Wait(Mutex& Guard);
		NEXUS_FRAMEWORK_API void Signal();
		NEXUS_FRAMEWORK_API void Broadcast();

	private:
		void* Handle;
	};
}
