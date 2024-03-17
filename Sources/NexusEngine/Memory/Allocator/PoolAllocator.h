#pragma once

#include "Core/NexusEngineCore.h"
#include "Allocator.h"

namespace NxEn
{
    class PoolAllocator : public Allocator
    {
    public:
        NEXUS_ENGINE_API PoolAllocator(uint64 Count, uint64 Stride);
        NEXUS_ENGINE_API ~PoolAllocator();

        NEXUS_ENGINE_API void* Allocate();
        NEXUS_ENGINE_API void Free(void* Pointer);
        NEXUS_ENGINE_API void Clear();

        NEXUS_ENGINE_API uint64 SlotAvailable() const;
        NEXUS_ENGINE_API bool CanAllocate() const;
        NEXUS_ENGINE_API bool ValidAddress(void* Pointer) const;

    private:
        uint64* Head;
        uint64 Stride;
    };
}

