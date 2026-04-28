#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Threading/Atomic.h"

namespace NxFr
{
	struct Thread
	{
		friend class Platform;

		enum class Status : int64
		{
			Uninitialized = 0,
			Running = 1 << 0,
			Finished = 1 << 1,
			Detached = 1 << 2,
			Joined = 1 << 3
		};

	public:
		NEXUS_FRAMEWORK_API static uint64 ThreadId();
		NEXUS_FRAMEWORK_API static uint64 MainThreadId();
		NEXUS_FRAMEWORK_API static bool IsMainThread();

		NEXUS_FRAMEWORK_API static void Yield();
		NEXUS_FRAMEWORK_API static void Sleep(uint64 Milliseconds);

		NEXUS_FRAMEWORK_API Thread(const NxFr::Delegate<void()>& Function);
		NEXUS_FRAMEWORK_API Thread(const Thread& Other) = delete;
		NEXUS_FRAMEWORK_API Thread(Thread&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~Thread();

		NEXUS_FRAMEWORK_API void Run();
		NEXUS_FRAMEWORK_API void Join();
		NEXUS_FRAMEWORK_API void Detach();

		NEXUS_FRAMEWORK_API bool HasStarted() const { return IsRunning() || IsFinished(); }
		NEXUS_FRAMEWORK_API bool IsFinished() const { return Integer::CheckFlag(State.Load(), (int64)Status::Finished); }
		NEXUS_FRAMEWORK_API bool IsRunning() const { return Integer::CheckFlag(State.Load(), (int64)Status::Running); }
		NEXUS_FRAMEWORK_API bool IsDetached() const { return Integer::CheckFlag(State.Load(), (int64)Status::Detached); }
		NEXUS_FRAMEWORK_API bool IsJoining()  const { return Integer::CheckFlag(State.Load(), (int64)Status::Joined); }

		NEXUS_FRAMEWORK_API NxFr::GUID GetId() const { return Id.Load(); }

	private:
		NEXUS_FRAMEWORK_API void RunOnThread();
		NEXUS_FRAMEWORK_API bool SetState(Status Target);

	private:
		NxFr::Delegate<void()> Function;
		Atomic State;
		Atomic Id;
		void* Handle;
	};
}
