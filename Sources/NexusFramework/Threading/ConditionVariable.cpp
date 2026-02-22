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
		Lock Context(Guard);
		while (!Predicate.Invoke())
		{
			Wait(Guard);
		}
	}

	void ConditionVariable::Wait(Mutex& Guard)
	{
		Platform::GetInstance()->ThreadConditionWait(Handle, Guard.Handle);
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
