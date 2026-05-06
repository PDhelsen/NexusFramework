#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Atomic.h"
#include "Fence.h"

namespace NxFr
{
	Atomic::Atomic(int64 Value)
		: Value(Value)
	{
	}

	Atomic::~Atomic()
	{
	}

	int64 Atomic::Add(int64 Target)
	{
		return Platform::GetInstance()->ThreadAtomicAdd(&Value, Target);
	}

	int64 Atomic::Increment()
	{
		return Platform::GetInstance()->ThreadAtomicIncrement(&Value);
	}

	int64 Atomic::Decrement()
	{
		return Platform::GetInstance()->ThreadAtomicDecrement(&Value);
	}

	int64 Atomic::Load() const
	{
		return Platform::GetInstance()->ThreadAtomicLoad(&Value);
	}

	void Atomic::Store(int64 Target)
	{
		Platform::GetInstance()->ThreadAtomicStore(&Value, Target);
	}

	bool Atomic::CompareExchange(int64 Target, int64 Expected)
	{
		return Platform::GetInstance()->ThreadAtomicCompareExchange(&Value, Target, Expected);
	}
}
