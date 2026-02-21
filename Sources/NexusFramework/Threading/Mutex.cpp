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
		Platform::GetInstance()->ThreadMutexLock(Handle);
	}

	void Mutex::Unlock()
	{
		Platform::GetInstance()->ThreadMutexUnlock(Handle);
	}

	Lock::Lock(Mutex& Target)
		: Target(Target)
	{
		Target.Lock();
	}

	Lock::~Lock()
	{
		Target.Unlock();
	}
}
