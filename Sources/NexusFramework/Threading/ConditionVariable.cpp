#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/ConditionVariable.h"

namespace NxFr
{
	ConditionVariable::ConditionVariable()
	{
		Handle = Platform::GetInstance()->ThreadConditionCreate();
	}

	ConditionVariable::~ConditionVariable()
	{
		Platform::GetInstance()->ThreadConditionDestroy(Handle);
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
		Platform::GetInstance()->ThreadConditionWait(Handle, Guard.Handle);
		Guard.Owner.Store(Thread::ThreadId());
	}

	void ConditionVariable::Signal()
	{
		Platform::GetInstance()->ThreadConditionSignal(Handle);
	}

	void ConditionVariable::Broadcast()
	{
		Platform::GetInstance()->ThreadConditionBroadcast(Handle);
	}
}
