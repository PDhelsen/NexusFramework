#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	class Allocator
	{
	public:
		NEXUS_ENGINE_API virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		NEXUS_ENGINE_API virtual void Free(void* Pointer) = 0;

		// If there is less than 4 bytes available in the stack, it is considered full
		NEXUS_ENGINE_API virtual bool IsFull() const { return FreeAmount() < 4; };
		NEXUS_ENGINE_API virtual uint64 UsedAmount() const { return Amount; };
		NEXUS_ENGINE_API virtual uint64 FreeAmount() const { return Capacity - Amount; };
		NEXUS_ENGINE_API virtual uint64 TotalAmount() const { return Capacity; };

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

