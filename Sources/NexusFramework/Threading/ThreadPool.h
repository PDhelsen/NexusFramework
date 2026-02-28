#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Threading/Thread.h"
#include "NexusFramework/Threading/Atomic.h"
#include "NexusFramework/Threading/Mutex.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	class ThreadPool
	{
	public:
		using Task = NxFr::Delegate<void()>;

		NEXUS_FRAMEWORK_API ThreadPool(uint64 Size);
		NEXUS_FRAMEWORK_API ThreadPool(const ThreadPool& Other) = delete;
		NEXUS_FRAMEWORK_API ThreadPool(ThreadPool&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~ThreadPool();

		NEXUS_FRAMEWORK_API ThreadPool& operator=(const ThreadPool& Other) = delete;
		NEXUS_FRAMEWORK_API ThreadPool& operator=(ThreadPool&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Submit(Task Work);
		NEXUS_FRAMEWORK_API void Wait();

		NEXUS_FRAMEWORK_API bool HasWorkPending();
		NEXUS_FRAMEWORK_API bool IsWorking();

	private:
		NEXUS_FRAMEWORK_API void Worker();

	private:
		NxFr::Array<Thread*> Threads;
		NxFr::Queue<Task> Tasks;
		Atomic Work;
		Atomic Running;

		Mutex Guard;
		ConditionVariable Notification;
	};
}

