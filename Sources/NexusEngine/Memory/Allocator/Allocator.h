#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	// Base class for custom allocator

	class Allocator
	{
	public:
		NEXUS_ENGINE_API bool IsFull() const { return Usage() > 0.95f; };
		NEXUS_ENGINE_API float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }

		NEXUS_ENGINE_API uint64 UsedAmount() const { return Amount; };
		NEXUS_ENGINE_API uint64 FreeAmount() const { return Capacity - Amount; };
		NEXUS_ENGINE_API uint64 TotalAmount() const { return Capacity; };

	protected:
		Allocator(uint64 Size);
		~Allocator();

		void WipeoutMemory();
		void EraseMemory(void* Memory, uint64 Size);

		void UpdateAmount(uint64 Delta, bool Increase) { Increase ? Amount += Delta : Amount -= Delta; }
		void ResetAmount() { Amount = 0; }

		void* GetMemoryBlock() const { return Memory; }

	private:
		uint64 Amount;
		uint64 Capacity;
		void* Memory;
	};
}

