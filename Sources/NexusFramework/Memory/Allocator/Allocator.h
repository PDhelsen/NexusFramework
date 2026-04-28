#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

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
		NEXUS_FRAMEWORK_API Allocator(uint64 Size);
		NEXUS_FRAMEWORK_API Allocator(const Allocator& Other) = delete;
		NEXUS_FRAMEWORK_API Allocator(Allocator&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API virtual ~Allocator() = default;

		NEXUS_FRAMEWORK_API Allocator& operator=(const Allocator& Other) = delete;
		NEXUS_FRAMEWORK_API Allocator& operator=(Allocator&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API virtual void Clear() = 0;
		NEXUS_FRAMEWORK_API virtual bool CanAllocate(uint64 Size, uint64 Alignement) const = 0;
		NEXUS_FRAMEWORK_API virtual bool BelongToAllocator(void* Pointer) const = 0;

		NEXUS_FRAMEWORK_API virtual uint64 UsedAmount() const { return Amount; };
		NEXUS_FRAMEWORK_API virtual uint64 FreeAmount() const { return Capacity > 0 ? Capacity - Amount : 0; };
		NEXUS_FRAMEWORK_API virtual uint64 TotalAmount() const { return Capacity; };
		NEXUS_FRAMEWORK_API virtual bool IsEmpty() const { return UsedAmount() == 0; };
		NEXUS_FRAMEWORK_API virtual bool IsFull() const { return Usage() > 0.95f; };
		NEXUS_FRAMEWORK_API virtual float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }

	protected:
		NEXUS_FRAMEWORK_API virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		NEXUS_FRAMEWORK_API virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) = 0;
		NEXUS_FRAMEWORK_API virtual void Free(void* Pointer) = 0;

		NEXUS_FRAMEWORK_API virtual void IncreaseAmount(uint64 Delta);
		NEXUS_FRAMEWORK_API virtual void DecreaseAmount(uint64 Delta);
		NEXUS_FRAMEWORK_API virtual void ResetAmount();

	private:
		uint64 Capacity;
		uint64 Amount;
	};
}

