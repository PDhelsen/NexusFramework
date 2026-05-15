#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Thread.h"

#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
	uint64 Thread::ThreadId()
	{
		return Globals::PlatformTarget->ThreadId();
	}

	void Thread::Yield()
	{
		Globals::PlatformTarget->ThreadYield();
	}

	void Thread::Sleep(uint64 Milliseconds)
	{
		Globals::PlatformTarget->ThreadSleep(Milliseconds);
	}

	Thread::Thread(const Delegate<void()>& Function)
		: Function(Function), Handle(nullptr), State(), Id()
	{
	}

	Thread::Thread(Thread&& Other) noexcept
		: Function(Move(Other.Function)), Handle(Other.Handle), State(Other.State.Load()), Id(Other.Id.Load())
	{
	}


	Thread::~Thread()
	{
		NEXUS_ASSERT(!IsRunning(), Default, "Thread is still running")

		Globals::PlatformTarget->ThreadDestroy(Handle);
	}

	void Thread::Run()
	{
		NEXUS_ASSERT(!HasStarted(), Default, "Thread is already running");

		if (SetState(Status::Running))
		{
			Handle = Globals::PlatformTarget->ThreadCreate(this);
		}
	}

	void Thread::RunOnThread()
	{
		Id.Store(ThreadId());
		Function.Invoke();
		SetState(Status::Finished);
	}

	void Thread::Join()
	{
		NEXUS_ASSERT(IsRunning() || IsFinished(), Default, "Thread is not running");
		NEXUS_ASSERT(!IsDetached(), Default, "Thread is already detached");

		if (SetState(Status::Joined))
		{
			Globals::PlatformTarget->ThreadJoin(Handle);
		}
	}

	void Thread::Detach()
	{
		NEXUS_ASSERT(IsRunning() || IsFinished(), Default, "Thread is not running");
		NEXUS_ASSERT(!IsJoining(), Default, "Thread is already joining");

		if (SetState(Status::Detached))
		{
			Globals::PlatformTarget->ThreadDetach(Handle);
		}
	}

	bool Thread::SetState(Status Target)
	{
		int64 Expected;
		int64 Desired;

		do
		{
			Expected = State.Load();

			if (Integer::CheckFlag(Expected, (int64)Target))
			{
				return false;
			}

			if (Target == Status::Running || Target == Status::Finished)
			{
				Desired = (Expected & ~((int64)Status::Running | (int64)Status::Finished)) |
					(int64)Target |
					(int64)(Integer::CheckFlag(Expected, (int64)Status::Detached) ? Status::Detached : Status::Uninitialized) |
					(int64)(Integer::CheckFlag(Expected, (int64)Status::Joined) ? Status::Joined : Status::Uninitialized);
			}
			else if (Target == Status::Detached || Target == Status::Joined)
			{
				Desired = (Expected & ((int64)Status::Running | (int64)Status::Finished)) |
					(int64)Target;
			}

		} while (!State.CompareExchange(Desired, Expected));

		return true;
	}
}
