#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
    class NX_FRAMEWORK_API HeapAllocator : public MemoryAllocator
    {
    public:
		NX_NOCOPY_NOMOVE(HeapAllocator)
		HeapAllocator(uint64 Size);
        virtual ~HeapAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

		bool IsEmpty() const override { return UsedAmount() == sizeof(HeapSlot); };

	protected:
		struct HeapSlot
		{
			HeapSlot* Next;
			bool Free;
		};

		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

        void UpdateHeapSlot(HeapSlot* Slot, uint64 Size);
        void RemoveNextHeapSlot(HeapSlot* Slot);
		HeapSlot* GetHeapSlot(void* Pointer) const;
		HeapSlot* GetHeapSlot(uint64 Size) const;
        void* GetHeapSlotMemory(HeapSlot* Slot) const;
		uint64 GetHeapSlotSize(HeapSlot* Slot) const;
		uint64 GetAlignedSize(uint64 Size) const;

		HeapSlot* Root;
		mutable HeapSlot* Cache;
    };
}

