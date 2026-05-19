#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/Queue.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Threading/Thread.h"
#include "NexusFramework/Threading/Atomic.h"
#include "NexusFramework/Threading/Mutex.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	class NX_FRAMEWORK_API ThreadPool
	{
	public:
		static uint64 MaxThreadCount();

		NX_NOCOPY_NOMOVE(ThreadPool)
		ThreadPool(uint64 Size = 0);
		~ThreadPool();

		void Dispatch(uint64 Count, uint64 Group, Delegate<void(uint64)> Work);
		void Dispatch(uint64 Count, Delegate<void(uint64)> Work);
		void Submit(const Delegate<void()>& Work);
		void Wait();

		bool HasWorkPending();
		bool IsWorking();

	private:
		void Worker();

		Array<Thread*> Threads;
		Queue<Delegate<void()>> Tasks;
		Atomic Work;
		Atomic Running;

		Mutex Guard;
		ConditionVariable Notification;
	};
}

