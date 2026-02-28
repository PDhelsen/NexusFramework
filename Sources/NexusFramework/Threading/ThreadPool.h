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
		struct Task
		{
		public:
			Task(const NxFr::Delegate<void()>& Function);
			void Release();

		public:
			NxFr::Delegate<void()> Function;
			Atomic Completed;

			Atomic RefCount;
			Mutex Guard;
			ConditionVariable Notification;
		};

	public:
		struct TaskHandle
		{
			friend class ThreadPool;

		public:
			NEXUS_FRAMEWORK_API TaskHandle(const TaskHandle& Other) = delete;
			NEXUS_FRAMEWORK_API TaskHandle(TaskHandle&& Other) noexcept;
			NEXUS_FRAMEWORK_API ~TaskHandle();

			NEXUS_FRAMEWORK_API void Wait();
			NEXUS_FRAMEWORK_API bool IsDone() const;

		private:
			NEXUS_FRAMEWORK_API TaskHandle(Task* State);

			Task* State;
		};

		NEXUS_FRAMEWORK_API ThreadPool(uint64 Size);
		NEXUS_FRAMEWORK_API ThreadPool(const ThreadPool& Other) = delete;
		NEXUS_FRAMEWORK_API ThreadPool(ThreadPool&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~ThreadPool();

		NEXUS_FRAMEWORK_API ThreadPool& operator=(const ThreadPool& Other) = delete;
		NEXUS_FRAMEWORK_API ThreadPool& operator=(ThreadPool&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API TaskHandle Submit(const NxFr::Delegate<void()>& Work);
		NEXUS_FRAMEWORK_API void Wait();

		NEXUS_FRAMEWORK_API bool HasWorkPending();
		NEXUS_FRAMEWORK_API bool IsWorking();

	private:
		NEXUS_FRAMEWORK_API void Worker();

	private:
		Array<Thread*> Threads;
		Queue<Task*> Tasks;
		Atomic Work;
		Atomic Running;

		Mutex Guard;
		ConditionVariable Notification;
	};
}

