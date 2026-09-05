#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	class NX_FRAMEWORK_API MemoryAllocator : public Allocator
	{
	public:
		NX_NOCOPY_NOMOVE(MemoryAllocator)
		MemoryAllocator(uint64 Size);
		virtual ~MemoryAllocator();

		uint64 UsedAmount() const override { return Amount; }
		uint64 FreeAmount() const override { return Capacity - Amount; }
		uint64 TotalAmount() const override { return Capacity; }
		float Usage() const override { return UsedAmount() / (float)TotalAmount(); }
		bool IsFull() const override { return Usage() > 0.95f; }
		bool IsEmpty() const override { return UsedAmount() == 0; }

	protected:
		void WipeoutMemory();
		void EraseMemory(void* Pointer, uint64 Size);
		bool IsPointerInMemoryBlock(void* Pointer) const;

		void* Data;
		uint64 Amount;
		uint64 Capacity;
	};
}
