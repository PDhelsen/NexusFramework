#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Atomic.h"

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

	void Atomic::Add(uint64 Target)
	{
		Platform::GetInstance()->ThreadAtomicAdd(&Value, Target);
	}

	void Atomic::Increment()
	{
		Platform::GetInstance()->ThreadAtomicIncrement(&Value);
	}

	void Atomic::Decrement()
	{
		Platform::GetInstance()->ThreadAtomicDecrement(&Value);
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

	void Atomic::Store(uint64 Target)
	{
		Platform::GetInstance()->ThreadAtomicStore(&Value, Target);
	}

	void Atomic::Store(bool Target)
	{
		Platform::GetInstance()->ThreadAtomicStore(&Value, Target ? 1 : 0);
	}
}
