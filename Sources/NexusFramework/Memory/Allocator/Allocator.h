#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	class Allocator;
	namespace Memory
	{
		NEXUS_FRAMEWORK_API void* Allocate(uint64, Allocator*, uint64);
		NEXUS_FRAMEWORK_API void* Reallocate(void*, uint64, Allocator*, uint64);
		NEXUS_FRAMEWORK_API void Free(void*, Allocator*);
	}

	class NEXUS_FRAMEWORK_API Allocator
	{
		friend void* Memory::Allocate(uint64, Allocator*, uint64);
		friend void* Memory::Reallocate(void*, uint64, Allocator*, uint64);
		friend void Memory::Free(void*, Allocator*);

	public:
		struct NEXUS_FRAMEWORK_API Scope
		{
		public:
			static Allocator* Get();
			static void Push(Allocator* Alloc);
			static void Pop();
			static void Reset();

			Scope(Allocator* Allocator);
			~Scope();
		};

		NEXUS_NOCOPY_NOMOVE(Allocator)
		Allocator(uint64 Size);
		virtual ~Allocator() = default;

		virtual void Clear() = 0;
		virtual bool CanAllocate(uint64 Size, uint64 Alignement) const = 0;
		virtual bool BelongToAllocator(void* Pointer) const = 0;

		virtual uint64 UsedAmount() const { return Amount; };
		virtual uint64 FreeAmount() const { return Capacity > 0 ? Capacity - Amount : 0; };
		virtual uint64 TotalAmount() const { return Capacity; };
		virtual bool IsEmpty() const { return UsedAmount() == 0; };
		virtual bool IsFull() const { return Usage() > 0.95f; };
		virtual float Usage() const { return (float)UsedAmount() / (float)TotalAmount(); }

	protected:
		virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) = 0;
		virtual void Free(void* Pointer) = 0;

		virtual void IncreaseAmount(uint64 Delta);
		virtual void DecreaseAmount(uint64 Delta);
		virtual void ResetAmount();

	private:
		uint64 Capacity;
		uint64 Amount;
	};
}

