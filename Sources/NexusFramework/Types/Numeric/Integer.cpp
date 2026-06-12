#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Misc/Hash/Hash.h"
#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Threading/Thread.h"

namespace NxFr
{
	namespace Integer
	{
		static thread_local GUID GlobalSeed = 0;

		GUID GenerateGuid()
		{
			GlobalSeed = GenerateGuid(Time::TimeSinceEpoch(), Globals::PlatformTarget->GetProcessId(), Thread::ThreadId(), GlobalSeed);
			return GlobalSeed;
		}

		GUID GenerateGuid(uint64 Time, uint64 ProcessId, uint64 ThreadId, uint64 Seed)
		{
			GUID Id = 0;
			while (Id == 0)
			{
				Hash Instance;
				Instance.Accumulate(Seed);
				Instance.Accumulate(Time);
				Instance.Accumulate(ProcessId);
				Instance.Accumulate(ThreadId);
				Id = Instance.Finalize();
			}
			return Id;
		}
	}
}
