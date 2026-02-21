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

	void ConditionVariable::Wait(Mutex& Target, const Delegate<bool()>& Predicate)
	{
		while (!Predicate.Invoke())
		{
			Wait(Target);
		}
	}

	void ConditionVariable::Wait(Mutex& Target)
	{
		Platform::GetInstance()->ThreadConditionWait(Handle, Target.Handle);
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
