#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Fence.h"

namespace NxFr
{
	Fence::Fence(uint64 Value)
		: Condition(), Guard(), Counter(Value)
	{
	}

	Fence::~Fence()
	{
	}

	void Fence::Wait()
	{
		Lock Context(Guard);
		Condition.Wait(Guard, [&]() { return Counter == 0; });
	}

	void Fence::Signal()
	{
		Lock Context(Guard);
		if (Counter > 0 && --Counter == 0)
		{
			Condition.Broadcast();
		}
	}

	void Fence::Set(uint64 Value)
	{
		Lock Context(Guard);
		Counter = Value;
	}

	uint64 Fence::Get()
	{
		Lock Context(Guard);
		return Counter;
	}
}
