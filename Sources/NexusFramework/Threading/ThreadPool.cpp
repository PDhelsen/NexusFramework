#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/ThreadPool.h"

namespace NxFr
{
	ThreadPool::Task::Task(const NxFr::Delegate<void()>& Function)
		: Function(Function), Completed(), RefCount(), Guard(), Notification()
	{
		Completed.Store(0);
		RefCount.Store(2);
	}

	void ThreadPool::Task::Release()
	{
		if (RefCount.Decrement() == 0)
		{
			delete this;
		}
	}

	ThreadPool::TaskHandle::TaskHandle(Task* State)
		: State(State)
	{
	}

	ThreadPool::TaskHandle::TaskHandle(TaskHandle&& Other) noexcept
		: State(Other.State)
	{
		Other.State = nullptr;
	}

	ThreadPool::TaskHandle::~TaskHandle()
	{
		if (State)
		{
			State->Release();
		}
	}

	void ThreadPool::TaskHandle::Wait()
	{
		if (!State)
		{
			return;
		}

		Lock GuardLock(State->Guard);
		State->Notification.Wait(State->Guard, [&]() { return State->Completed.Load(); });
	}

	bool ThreadPool::TaskHandle::IsDone() const
	{
		if (!State)
		{
			return true;
		}

		return State->Completed.Load();
	}

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

	ThreadPool::TaskHandle ThreadPool::Submit(const NxFr::Delegate<void()>& Work)
	{
		Task* State = new Task(Work);
		{
			Lock GuardLock(Guard);
			Tasks.Append(State);
		}
		Notification.Signal();

		return TaskHandle(State);
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
			Task* Instance;

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

			Instance->Function.Invoke();
			{
				Lock GuardLock(Instance->Guard);
				Instance->Completed.Store(1);
			}
			Instance->Notification.Broadcast();
			Instance->Release();

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
