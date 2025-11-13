#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
	namespace Integer
	{
		GUID GenerateGuid()
		{
			return GenerateGuid(Time::GetTimeSinceEpoch(), Platform::GetInstance()->GetProcessId());
		}

		GUID GenerateGuid(uint64 Time, uint64 ProcessId)
		{
			Hasher Instance;
			Instance.Accumulate(Time);
			Instance.Accumulate(ProcessId);
			return Instance.Hash();
		}
	}
}
