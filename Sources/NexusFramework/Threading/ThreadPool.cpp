#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/ThreadPool.h"

#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
	uint64 ThreadPool::MaxThreadCount()
	{
		return Math::Max(1llu, Globals::PlatformTarget->GetProcessorCount() - 1);
	}

	ThreadPool::ThreadPool(uint64 Size)
		: Threads(), Tasks(), Work(), Running(), Guard(), Notification()
	{
		Threads = Size != 0 ? Size : MaxThreadCount();
		Running.Store(1);

		for (uint64 Index = 0; Index < Threads.GetCount(); ++Index)
		{
			Threads[Index] = new Thread({ this, &ThreadPool::Worker });
			Threads[Index]->Run();
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			Lock GuardLock(Guard);
			Running.Store(0);
		}

		Notification.Broadcast();

		for (uint64 Index = 0; Index < Threads.GetCount(); ++Index)
		{
			Threads[Index]->Join();
			delete Threads[Index];
		}
	}

	void ThreadPool::Dispatch(uint64 Count, uint64 Group, NxFr::Delegate<void(uint64)> Work)
	{
		uint64 Batch = (Count + Group - 1) / Group;
		for (uint64 Index = 0; Index < Batch; ++Index)
		{
			Submit([=]()
			{
				uint64 Begin = Index * Group;
				uint64 End = Math::Min(Begin + Group, Count);
				for (uint64 It = Begin; It < End; ++It)
				{
					Work(It);
				}

			});
		}
	}

	void ThreadPool::Dispatch(uint64 Count, NxFr::Delegate<void(uint64)> Work)
	{
		for (uint64 Index = 0; Index < Count; ++Index)
		{
			Submit([=]()
			{
				Work(Index);
			});
		}
	}

	void ThreadPool::Submit(const NxFr::Delegate<void()>& Work)
	{
		Lock GuardLock(Guard);

		Tasks.Append(Work);
		Notification.Signal();
	}

	void ThreadPool::Wait()
	{
		Lock GuardLock(Guard);

		Notification.Wait(Guard, [&](){ return Tasks.IsEmpty() && !Work.Load(); });
	}

	bool ThreadPool::HasWorkPending()
	{
		Lock GuardLock(Guard);

		return !Tasks.IsEmpty();
	}

	bool ThreadPool::IsWorking()
	{
		Lock GuardLock(Guard);

		return Work.Load();
	}

	void ThreadPool::Worker()
	{
		while (true)
		{
			NxFr::Delegate<void()> Instance(nullptr);

			{
				Lock GuardLock(Guard);

				Notification.Wait(Guard, [&]() { return !Running.Load() || !Tasks.IsEmpty(); });
				if (!Running.Load() && Tasks.IsEmpty())
				{
					return;
				}

				Instance = Tasks.Get();
				Tasks.Remove();

				Work.Increment();
			}

			Instance();

			{
				Lock GuardLock(Guard);

				Work.Decrement();
				if (Tasks.IsEmpty() && !Work.Load())
				{
					Notification.Broadcast();
				}
			}
		}
	}
}
