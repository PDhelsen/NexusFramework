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

    struct HeapSlot
    {
        HeapSlot* Next;
        bool Free;
    };

    class HeapAllocator : public Allocator
    {
    public:
        NEXUS_ENGINE_API HeapAllocator(uint64 Size);
        NEXUS_ENGINE_API ~HeapAllocator();

        NEXUS_ENGINE_API void* Allocate(uint64 Size = 0, uint64 Alignement = 0) override;
        NEXUS_ENGINE_API void Free(void* Pointer);
        NEXUS_ENGINE_API void Clear();

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size = 0, uint64 Alignement = 0) const override;
        NEXUS_ENGINE_API bool IsValidAddress(void* Pointer) const;

        NEXUS_ENGINE_API void Defragment();

    private:
        HeapSlot* GetHeapSlot(uint64 Size) const;
        uint64 GetAlignedSize(uint64 Size) const;
        uint64 GetHeapSlotSize(HeapSlot* Slot) const;
        uint8* GetHeapSlotData(HeapSlot* Slot) const;

        HeapSlot* Root;
    };
}

