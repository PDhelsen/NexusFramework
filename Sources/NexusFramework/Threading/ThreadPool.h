#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/Queue.h"
#include "NexusFramework/Threading/Thread.h"
#include "NexusFramework/Threading/Atomic.h"
#include "NexusFramework/Threading/Mutex.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	class ThreadPool
	{
	public:
		NEXUS_FRAMEWORK_API static uint64 MaxThreadCount();

		NEXUS_FRAMEWORK_API ThreadPool(uint64 Size = 0);
		NEXUS_FRAMEWORK_API ThreadPool(const ThreadPool& Other) = delete;
		NEXUS_FRAMEWORK_API ThreadPool(ThreadPool&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~ThreadPool();

		NEXUS_FRAMEWORK_API ThreadPool& operator=(const ThreadPool& Other) = delete;
		NEXUS_FRAMEWORK_API ThreadPool& operator=(ThreadPool&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Dispatch(uint64 Count, uint64 Group, NxFr::Delegate<void(uint64)> Work);
		NEXUS_FRAMEWORK_API void Dispatch(uint64 Count, NxFr::Delegate<void(uint64)> Work);
		NEXUS_FRAMEWORK_API void Submit(NxFr::Delegate<void()> Work);
		NEXUS_FRAMEWORK_API void Wait();

		NEXUS_FRAMEWORK_API bool HasWorkPending();
		NEXUS_FRAMEWORK_API bool IsWorking();

	private:
		NEXUS_FRAMEWORK_API void Worker();

	private:
		Array<Thread*> Threads;
		Queue<NxFr::Delegate<void()>> Tasks;
		Atomic Work;
		Atomic Running;

		Mutex Guard;
		ConditionVariable Notification;
	};
}

