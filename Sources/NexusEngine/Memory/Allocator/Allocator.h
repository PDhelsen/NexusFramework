#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	class Allocator
	{
	public:
		NEXUS_ENGINE_API bool IsFull() const { return Usage() > 0.95f; };
		NEXUS_ENGINE_API float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }
		NEXUS_ENGINE_API uint64 UsedAmount() const { return Amount; };
		NEXUS_ENGINE_API uint64 FreeAmount() const { return Capacity - Amount; };
		NEXUS_ENGINE_API uint64 TotalAmount() const { return Capacity; };

	protected:
		NEXUS_ENGINE_API Allocator(uint64 Size);
		NEXUS_ENGINE_API ~Allocator();

		NEXUS_ENGINE_API void SetAmount(uint64 Used) { Amount = Used; }
		NEXUS_ENGINE_API void* GetPointer() const { return Memory; }

	private:
		uint64 Amount;
		uint64 Capacity;
		void* Memory;
	};
}

