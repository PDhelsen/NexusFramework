#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	class Allocator
	{
	public:
		NEXUS_ENGINE_API virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		NEXUS_ENGINE_API virtual void Free(void* Pointer) = 0;
		NEXUS_ENGINE_API virtual void Clear() = 0;

		NEXUS_ENGINE_API virtual bool CanFit(uint64 Size, uint64 Alignement) const = 0;
		NEXUS_ENGINE_API virtual bool ValidAddress(void* Pointer) const = 0;

		NEXUS_ENGINE_API bool IsFull() const { return FreeAmount() == 0; };
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

