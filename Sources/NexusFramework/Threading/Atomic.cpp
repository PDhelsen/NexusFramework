#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Atomic.h"
#include "Fence.h"

namespace NxFr
{
	Atomic::Atomic()
		: Value(0)
	{
	}

	Atomic::Atomic(bool Value)
		: Value(Value ? 1 : 0)
	{
	}
	Atomic::Atomic(uint64 Value)
		: Value(Value)
	{
	}
	Atomic::~Atomic()
	{
	}

	Atomic::operator uint64()
	{
		return Load();
	}

	Atomic::operator bool()
	{
		return Is();
	}

	uint64 Atomic::Add(uint64 Target)
	{
		Platform::GetInstance()->ThreadAtomicAdd(&Value, Target);
		return Value;
	}

	uint64 Atomic::Increment()
	{
		Platform::GetInstance()->ThreadAtomicIncrement(&Value);
		return Value;
	}

	uint64 Atomic::Decrement()
	{
		Platform::GetInstance()->ThreadAtomicDecrement(&Value);
		return Value;
	}

	uint64 Atomic::Load()
	{
		Platform::GetInstance()->ThreadAtomicLoad(&Value);
		return Value;
	}

	bool Atomic::Is()
	{
		Platform::GetInstance()->ThreadAtomicLoad(&Value);
		return Value != 0;
	}

	uint64 Atomic::Store(uint64 Target)
	{
		Platform::GetInstance()->ThreadAtomicStore(&Value, Target);
		return Value;
	}

	bool Atomic::Store(bool Target)
	{
		Platform::GetInstance()->ThreadAtomicStore(&Value, Target ? 1 : 0);
		return Value != 0;
	}
}
