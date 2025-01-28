#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"

namespace NxFr
{
	class Allocator;
	namespace Memory
	{
		NEXUS_FRAMEWORK_API void* Allocate(uint64, Allocator*, uint64);
		NEXUS_FRAMEWORK_API void* Reallocate(void*, uint64, Allocator*, uint64);
		NEXUS_FRAMEWORK_API void Free(void*, Allocator*);
	}

	class Allocator
	{
		friend void* Memory::Allocate(uint64, Allocator*, uint64);
		friend void* Memory::Reallocate(void*, uint64, Allocator*, uint64);
		friend void Memory::Free(void*, Allocator*);

	public:
		Allocator(uint64 Size);
		Allocator(const Allocator& Other) = delete;
		Allocator(Allocator&& Other) noexcept = delete;
		virtual ~Allocator();

		Allocator& operator=(const Allocator& Other) = delete;
		Allocator& operator=(Allocator&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API virtual void Clear() = 0;
		NEXUS_FRAMEWORK_API virtual bool CanAllocate(uint64 Size, uint64 Alignement) const = 0;
		NEXUS_FRAMEWORK_API virtual bool BelongToAllocator(void* Pointer) const = 0;

		NEXUS_FRAMEWORK_API bool IsFull() const { return Usage() > 0.95f; };
		NEXUS_FRAMEWORK_API float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }
		NEXUS_FRAMEWORK_API uint64 UsedAmount() const { return Amount; };
		NEXUS_FRAMEWORK_API uint64 FreeAmount() const { return Amount < Capacity ? Capacity - Amount : 0; };
		NEXUS_FRAMEWORK_API uint64 TotalAmount() const { return Capacity; };

	protected:
		virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) = 0;
		virtual void Free(void* Pointer) = 0;

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

