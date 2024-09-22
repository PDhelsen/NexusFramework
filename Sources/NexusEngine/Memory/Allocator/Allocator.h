#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Memory/Memory.h"

namespace NxEn
{
	// Base class for custom allocator

	class Allocator;
	class HandleManager;

	struct AllocatorActive
	{
	public:
		NEXUS_ENGINE_API AllocatorActive(Allocator* Allocator);
		NEXUS_ENGINE_API ~AllocatorActive();
	};

	class Allocator
	{
		// TODO: Implementation - Handle - Remove once HandleManager replace PoolAllocator by pool container
		friend HandleManager;

	public:
		NEXUS_ENGINE_API virtual void* Allocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) = 0;
		NEXUS_ENGINE_API virtual void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) = 0;
		NEXUS_ENGINE_API virtual void Free(void* Pointer) = 0;
		NEXUS_ENGINE_API virtual void Clear() = 0;

		NEXUS_ENGINE_API virtual bool CanAllocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) const = 0;
		NEXUS_ENGINE_API virtual bool IsAllocatedAddress(void* Pointer) const = 0;

		NEXUS_ENGINE_API bool IsFull() const { return Usage() > 0.95f; };
		NEXUS_ENGINE_API float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }

		NEXUS_ENGINE_API uint64 UsedAmount() const { return Amount; };
		NEXUS_ENGINE_API uint64 FreeAmount() const { return Amount < Capacity ? Capacity - Amount : 0; };
		NEXUS_ENGINE_API uint64 TotalAmount() const { return Capacity; };

	protected:
		Allocator(uint64 Size);
		Allocator(const Allocator& Other) = delete;
		Allocator(Allocator&& Other) noexcept = delete;
		~Allocator();

		Allocator& operator=(const Allocator& Other) = delete;
		Allocator& operator=(Allocator&& Other) noexcept = delete;

		void WipeoutMemory();
		void EraseMemory(void* Memory, uint64 Size);

		void UpdateAmount(uint64 Delta, bool Increase) { Increase ? Amount += Delta : Amount -= Delta; }
		void ResetAmount() { Amount = 0; }

		bool IsPointerInside(void* Pointer) const;

		void* GetMemoryBlock() const { return Memory; }

	private:
		uint64 Amount;
		uint64 Capacity;
		void* Memory;
	};
}

