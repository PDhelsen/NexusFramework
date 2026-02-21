#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Threading/Atomic.h"

namespace NxFr
{
	struct Thread
	{
		enum class Status : uint64
		{
			Uninitialized, Running, Detached, Joined, Finished
		};

	public:
		NEXUS_FRAMEWORK_API static uint64 GetId();
		NEXUS_FRAMEWORK_API static void Yield();
		NEXUS_FRAMEWORK_API static void Sleep(uint64 Milliseconds);

		NEXUS_FRAMEWORK_API Thread(const NxFr::Delegate<void()>& Function, bool AutoStart = true);
		NEXUS_FRAMEWORK_API Thread(const Thread& Other) = delete;
		NEXUS_FRAMEWORK_API Thread(Thread&& Other) = default;
		NEXUS_FRAMEWORK_API ~Thread();

		NEXUS_FRAMEWORK_API void Start();
		NEXUS_FRAMEWORK_API void Stop();
		NEXUS_FRAMEWORK_API void Run();

		NEXUS_FRAMEWORK_API void Join();
		NEXUS_FRAMEWORK_API void Detach();

		NEXUS_FRAMEWORK_API bool HasStarted() const { return (State.Load() != ((uint64)(Status::Uninitialized))); }
		NEXUS_FRAMEWORK_API bool IsFinished() const { return State.Load() == (uint64)Status::Finished; }
		NEXUS_FRAMEWORK_API bool IsRunning() const { return HasStarted() && !IsFinished(); }
		NEXUS_FRAMEWORK_API bool IsDetached() const { return State.Load() == (uint64)Status::Detached; }
		NEXUS_FRAMEWORK_API bool IsJoining()  const { return State.Load() == (uint64)Status::Joined; }

	private:
		NxFr::Delegate<void()> Function;
		void* Handle;
		mutable Atomic State;
	};
}
