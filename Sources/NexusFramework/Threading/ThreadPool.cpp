#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/ThreadPool.h"

namespace NxFr
{
	ThreadPool::ThreadPool(uint64 Size)
		: Threads(Size), Tasks(), Work(), Running(), Guard(), Notification()
	{
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

	void ThreadPool::Submit(Task Work)
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
			Task Instance;

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

			Instance.Invoke();

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
