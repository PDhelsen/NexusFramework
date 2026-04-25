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
		NEXUS_ASSERT(Owner.Load() == 0, Default, "Mutex is locked by another thread");
		Platform::GetInstance()->ThreadMutexDestroy(Handle);
	}

	void Mutex::Lock()
	{
		NEXUS_ASSERT(Owner.Load() == 0 || Owner.Load() != Thread::ThreadId(), Default, "Mutex is already locked by this thread");
		Platform::GetInstance()->ThreadMutexLock(Handle);
		Owner.Store(Thread::ThreadId());
	}

	void Mutex::Unlock()
	{
		NEXUS_ASSERT(Owner.Load() == Thread::ThreadId(), Default, "Mutex is locked by another thread");
		Owner.Store(0);
		Platform::GetInstance()->ThreadMutexUnlock(Handle);
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
