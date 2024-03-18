#pragma once

#include "Core/NexusEngineCore.h"
#include "Allocator.h"

namespace NxEn
{
    class HeapAllocator : public Allocator
    {
        struct HeapSlot
        {
            HeapSlot* Previous;
            HeapSlot* Next;
            uint64 Size;
            bool Free;
        };

    public:
        NEXUS_ENGINE_API HeapAllocator(uint64 Size);
        NEXUS_ENGINE_API ~HeapAllocator();

        NEXUS_ENGINE_API void* Allocate(uint64 Size);
        NEXUS_ENGINE_API void Free(void* Pointer);
        NEXUS_ENGINE_API void Clear();

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size) const;
        NEXUS_ENGINE_API bool ValidAddress(void* Pointer) const;

    private:
        HeapSlot* GetHeapSlot(uint64 Size) const;

        HeapSlot* Root;
    };
}

