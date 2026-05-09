#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	ConditionVariable::ConditionVariable()
	{
		Handle = Globals::PlatformTarget->ThreadConditionCreate();
	}

	ConditionVariable::~ConditionVariable()
	{
		Globals::PlatformTarget->ThreadConditionDestroy(Handle);
	}

	void ConditionVariable::Wait(Mutex& Guard, const Delegate<bool()>& Predicate)
	{
		while (!Predicate.Invoke())
		{
			Wait(Guard);
		}
	}

	void ConditionVariable::Wait(Mutex& Guard)
	{
		Guard.Owner.Store(0);
		Globals::PlatformTarget->ThreadConditionWait(Handle, Guard.Handle);
		Guard.Owner.Store(Thread::ThreadId());
	}

	void ConditionVariable::Signal()
	{
		Globals::PlatformTarget->ThreadConditionSignal(Handle);
	}

	void ConditionVariable::Broadcast()
	{
		Globals::PlatformTarget->ThreadConditionBroadcast(Handle);
	}
}
