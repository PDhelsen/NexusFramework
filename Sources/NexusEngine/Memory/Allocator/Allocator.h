#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"

namespace NxEn
{
	// Base class for custom allocator

	class Allocator;

	struct AllocatorActive
	{
	public:
		NEXUS_ENGINE_API AllocatorActive(Allocator* Allocator);
		NEXUS_ENGINE_API ~AllocatorActive();
	};

	class Allocator
	{
	public:
		NEXUS_ENGINE_API virtual void* Allocate(uint64 Size = 0, uint64 Alignement = 0) = 0;
		NEXUS_ENGINE_API virtual void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = 0) = 0;
		NEXUS_ENGINE_API virtual void Free(void* Pointer) = 0;
		NEXUS_ENGINE_API virtual void Clear() = 0;

		NEXUS_ENGINE_API virtual bool CanAllocate(uint64 Size = 0, uint64 Alignement = 0) const = 0;
		NEXUS_ENGINE_API virtual bool IsValidAddress(void* Pointer) const = 0;

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

		// TEMP: Remove - Container - Once HandleManager replace PoolAllocator by pool container
		friend class HandleManager;
	};
}

