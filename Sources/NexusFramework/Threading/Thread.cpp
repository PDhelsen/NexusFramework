#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Thread.h"

namespace NxFr
{
	uint64 Thread::GetId()
	{
		return Platform::GetInstance()->ThreadId();
	}

	void Thread::Yield()
	{
		Platform::GetInstance()->ThreadYield();
	}

	void Thread::Sleep(uint64 Milliseconds)
	{
		Platform::GetInstance()->ThreadSleep(Milliseconds);
	}

	Thread::Thread(const NxFr::Delegate<void()>& Function, bool AutoStart)
		: Function(Function), Handle(nullptr), State()
	{
		if (AutoStart)
		{
			Start();
		}
	}

	Thread::~Thread()
	{
		if (IsRunning())
		{
			if (!IsDetached())
			{
				Join();
			}

			Stop();
		}
	}

	void Thread::Start()
	{
		NEXUS_ASSERT(State.Load() == (uint64)Status::Uninitialized, Default, "Thread has already started");
		Handle = Platform::GetInstance()->ThreadCreate(this);
	}

	void Thread::Stop()
	{
		NEXUS_ASSERT(State.Load() == (uint64)Status::Finished, Default, "Thread is still running");
		Platform::GetInstance()->ThreadDestroy(Handle);
	}

	void Thread::Run()
	{
		NEXUS_ASSERT(State.Load() == (uint64)Status::Uninitialized, Default, "Thread is already running");
		State.Store((uint64)Status::Running);
		Function.Invoke();
		State.Store((uint64)Status::Finished);
	}

	void Thread::Join()
	{
		if (IsFinished())
		{
			return;
		}

		NEXUS_ASSERT(State.Load() == (uint64)Status::Running, Default, "Thread is not running");
		State.Store((uint64)Status::Joined);
		Platform::GetInstance()->ThreadJoin(Handle);
	}

	void Thread::Detach()
	{
		if (IsFinished())
		{
			return;
		}

		NEXUS_ASSERT(State.Load() == (uint64)Status::Running, Default, "Thread is not running");
		State.Store((uint64)Status::Detached);
		Platform::GetInstance()->ThreadDetach(Handle);
	}
}
