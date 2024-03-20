#pragma once

#include "Core/NexusEngineCore.h"
#include "Allocator.h"

namespace NxEn
{
    // Pool allocator
    // Allow to store object from same size into a pool (doesn't need to be the same object type)
    // Implicit alignement since everything has the same size

    class PoolAllocator : public Allocator
    {
    public:
        NEXUS_ENGINE_API PoolAllocator(uint64 Count, uint64 Stride);
        NEXUS_ENGINE_API ~PoolAllocator();

        NEXUS_ENGINE_API void* Allocate();
        NEXUS_ENGINE_API void Free(void* Pointer);
        NEXUS_ENGINE_API void Clear();

        NEXUS_ENGINE_API bool CanAllocate() const;
        NEXUS_ENGINE_API bool IsValidAddress(void* Pointer) const;

        NEXUS_ENGINE_API uint64 SlotAvailable() const;

    private:
        uint64* Head;
        uint64 Stride;
    };
}

