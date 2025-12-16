#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
	namespace Integer
	{
		static GUID GlobalSeed = 0;

		GUID GenerateGuid()
		{
			return GenerateGuid(Time::GetTimeSinceEpoch(), Platform::GetInstance()->GetProcessId(), GlobalSeed);
		}

		GUID GenerateGuid(uint64 Time, uint64 ProcessId, uint64 Seed)
		{
			Hasher Instance;
			Instance.Accumulate(Seed);
			Instance.Accumulate(Time);
			Instance.Accumulate(ProcessId);
			GlobalSeed = Instance.Hash();
			return GlobalSeed;
		}
	}
}
