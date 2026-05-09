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
		return Globals::PlatformTarget->ThreadAtomicAdd(&Value, Target);
	}

	int64 Atomic::Increment()
	{
		return Globals::PlatformTarget->ThreadAtomicIncrement(&Value);
	}

	int64 Atomic::Decrement()
	{
		return Globals::PlatformTarget->ThreadAtomicDecrement(&Value);
	}

	int64 Atomic::Load() const
	{
		return Globals::PlatformTarget->ThreadAtomicLoad(&Value);
	}

	void Atomic::Store(int64 Target)
	{
		Globals::PlatformTarget->ThreadAtomicStore(&Value, Target);
	}

	bool Atomic::CompareExchange(int64 Target, int64 Expected)
	{
		return Globals::PlatformTarget->ThreadAtomicCompareExchange(&Value, Target, Expected);
	}
}
