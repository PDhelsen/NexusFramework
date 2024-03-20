#pragma once

#include "Core/NexusEngineCore.h"
#include "Allocator.h"

namespace NxEn
{
    // Heap allocator
    // General purpose allocator return an aligned pointer
    // The memory book keeping is handle by the HeapSlot struct stored just before the returned memory pointer

    // Since the memory pointer is aligned on 16 bytes and the HeapSlot is 32 bytes long,
    // Any address just after the heapslot is aligned on 16 bytes (32 = 16 * 2)
    // It should look like :
    // Root (16 aligned) + Heap slot (16 aligned) + Memory (Forced 16 aligned) + Heap slot (16 aligned) + Memory (Forced 16 aligned) + ... 

    struct HeapSlot
    {
        HeapSlot* Previous;
        HeapSlot* Next;
        uint64 Size;
        bool Free;
    };

    class HeapAllocator : public Allocator
    {
    public:
        NEXUS_ENGINE_API HeapAllocator(uint64 Size);
        NEXUS_ENGINE_API ~HeapAllocator();

        NEXUS_ENGINE_API void* Allocate(uint64 Size);
        NEXUS_ENGINE_API void Free(void* Pointer);
        NEXUS_ENGINE_API void Clear();

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size) const;
        NEXUS_ENGINE_API bool IsValidAddress(void* Pointer) const;

    private:
        HeapSlot* GetHeapSlot(uint64 Size) const;

        HeapSlot* Root;
    };
}

