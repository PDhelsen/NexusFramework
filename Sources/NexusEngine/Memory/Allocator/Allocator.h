#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"

namespace NxEn
{
	// Base class for custom allocator

	class Allocator
	{
	public:
		Allocator(uint64 Size);
		Allocator(const Allocator& Other) = delete;
		Allocator(Allocator&& Other) noexcept = delete;
		virtual ~Allocator();

		Allocator& operator=(const Allocator& Other) = delete;
		Allocator& operator=(Allocator&& Other) noexcept = delete;

		NEXUS_ENGINE_API virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		NEXUS_ENGINE_API virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) = 0;
		NEXUS_ENGINE_API virtual void Free(void* Pointer) = 0;

		NEXUS_ENGINE_API virtual void Clear() = 0;
		NEXUS_ENGINE_API virtual bool CanAllocate(uint64 Size, uint64 Alignement) const = 0;
		NEXUS_ENGINE_API virtual bool BelongToAllocator(void* Pointer) const = 0;

		NEXUS_ENGINE_API bool IsFull() const { return Usage() > 0.95f; };
		NEXUS_ENGINE_API float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }
		NEXUS_ENGINE_API uint64 UsedAmount() const { return Amount; };
		NEXUS_ENGINE_API uint64 FreeAmount() const { return Amount < Capacity ? Capacity - Amount : 0; };
		NEXUS_ENGINE_API uint64 TotalAmount() const { return Capacity; };

	protected:
		void IncreaseAmount(uint64 Delta);
		void DecreaseAmount(uint64 Delta);
		void ResetAmount();

		void WipeoutMemory();
		void EraseMemory(void* Pointer, uint64 Size);

		bool IsPointerInMemoryBlock(void* Pointer) const;
		void* GetMemoryBlock() const;

	private:
		uint64 Capacity;
		uint64 Amount;
		void* Data;
	};
}

