#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Threading/Thread.h"
#include "NexusFramework/Threading/ThreadPool.h"

namespace NxFr
{
	namespace Threading
	{
		NEXUS_FRAMEWORK_API bool IsMainThread();
		NEXUS_FRAMEWORK_API uint64 MainThreadId();
		NEXUS_FRAMEWORK_API uint64 MaxThreadCount();

		NEXUS_FRAMEWORK_API void Dispatch(uint64 Count, Delegate<void(uint64)> Function);
		NEXUS_FRAMEWORK_API void Dispatch(uint64 Count, uint64 Size, const Delegate<void(uint64)>& Function);
	}
}
