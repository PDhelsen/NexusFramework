#pragma once

#include "Allocator.h"

namespace NxEn
{
    // Heap allocator
    // General purpose allocator return an aligned pointer
    // The memory book keeping is handle by the HeapSlot struct stored just before the returned memory pointer

    // Since the memory pointer is aligned on 16 bytes and the HeapSlot is 16 bytes long,
    // Any address just after the heapslot is aligned on 16 bytes
    // It should look like :
    // Root (16 aligned) + Heap slot (16 aligned) + Memory (Forced 16 aligned) + Heap slot (16 aligned) + Memory (Forced 16 aligned) + ... 

    class HeapAllocator : public Allocator
    {
	private:
		struct HeapSlot
		{
			HeapSlot* Next;
			bool Free;
		};

    public:
        NEXUS_ENGINE_API HeapAllocator(uint64 Size);
		NEXUS_ENGINE_API HeapAllocator(const HeapAllocator& Other) = delete;
		NEXUS_ENGINE_API HeapAllocator(HeapAllocator&& Other) noexcept = delete;
        NEXUS_ENGINE_API virtual ~HeapAllocator();

		NEXUS_ENGINE_API HeapAllocator& operator=(const HeapAllocator& Other) = delete;
		NEXUS_ENGINE_API HeapAllocator& operator=(HeapAllocator&& Other) noexcept = delete;

		NEXUS_ENGINE_API void* Allocate(uint64 Size, uint64 Alignement) override;
		NEXUS_ENGINE_API void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		NEXUS_ENGINE_API void Free(void* Pointer) override;

		NEXUS_ENGINE_API void Clear() override;
		NEXUS_ENGINE_API bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		NEXUS_ENGINE_API bool BelongToAllocator(void* Pointer) const override;

		NEXUS_ENGINE_API void Defragment(uint64 Count = 0);

    private:
        void UpdateHeapSlot(HeapSlot* Slot, uint64 Size);
        void RemoveNextHeapSlot(HeapSlot* Slot);
		HeapSlot* GetHeapSlot(void* Pointer) const;
		HeapSlot* GetHeapSlot(uint64 Size) const;
        void* GetHeapSlotMemory(HeapSlot* Slot) const;
		uint64 GetHeapSlotSize(HeapSlot* Slot) const;
		uint64 GetAlignedSize(uint64 Size) const;
		void Reset();

        HeapSlot* Root;
    };
}

