#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	Mutex::Mutex()
	{
		Handle = Platform::GetInstance()->ThreadMutexCreate();
	}

	Mutex::~Mutex()
	{
		Platform::GetInstance()->ThreadMutexDestroy(Handle);
	}

	void Mutex::Lock()
	{
		if (State.Is())
		{
			return;
		}

		State.Store(true);
		Platform::GetInstance()->ThreadMutexLock(Handle);
	}

	void Mutex::Unlock()
	{
		if (!State.Is())
		{
			return;
		}

		Platform::GetInstance()->ThreadMutexUnlock(Handle);
		State.Store(false);
	}

	Lock::Lock(Mutex& Guard)
		: Guard(Guard)
	{
		Guard.Lock();
	}

	Lock::~Lock()
	{
		Guard.Unlock();
	}
}
