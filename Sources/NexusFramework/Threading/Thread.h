#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Threading/Atomic.h"

namespace NxFr
{
	struct NEXUS_FRAMEWORK_API Thread
	{
		friend class Platform;

	public:
		static uint64 ThreadId();
		static void Yield();
		static void Sleep(uint64 Milliseconds);

		NEXUS_NOCOPY(Thread)
		Thread(const NxFr::Delegate<void()>& Function);
		Thread(Thread&& Other) noexcept;
		~Thread();

		void Run();
		void Join();
		void Detach();

		bool HasStarted() const { return IsRunning() || IsFinished(); }
		bool IsFinished() const { return Integer::CheckFlag(State.Load(), (int64)Status::Finished); }
		bool IsRunning() const { return Integer::CheckFlag(State.Load(), (int64)Status::Running); }
		bool IsDetached() const { return Integer::CheckFlag(State.Load(), (int64)Status::Detached); }
		bool IsJoining()  const { return Integer::CheckFlag(State.Load(), (int64)Status::Joined); }

		NxFr::GUID GetId() const { return Id.Load(); }

	private:
		enum class Status : int64
		{
			Uninitialized = 0,
			Running = 1 << 0,
			Finished = 1 << 1,
			Detached = 1 << 2,
			Joined = 1 << 3
		};

		void RunOnThread();
		bool SetState(Status Target);

		NxFr::Delegate<void()> Function;
		Atomic State;
		Atomic Id;
		void* Handle;
	};
}
