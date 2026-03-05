#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/Threading/Thread.h"

namespace NxFr
{
	namespace Integer
	{
		static thread_local GUID GlobalSeed = 0;

		GUID GenerateGuid()
		{
			return GenerateGuid(Time::GetTimeSinceEpoch(), Platform::GetInstance()->GetProcessId(), Thread::ThreadId(), GlobalSeed);
		}

		GUID GenerateGuid(uint64 Time, uint64 ProcessId, uint64 ThreadId, uint64 Seed)
		{
			Hasher Instance;
			Instance.Accumulate(Seed);
			Instance.Accumulate(Time);
			Instance.Accumulate(ProcessId);
			Instance.Accumulate(ThreadId);
			GlobalSeed = Instance.Hash();
			return GlobalSeed;
		}
	}
}
