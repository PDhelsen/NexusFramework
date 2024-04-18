#pragma once

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

        NEXUS_ENGINE_API void* Allocate(uint64 Size = 0, uint64 Alignement = 0) override;
        NEXUS_ENGINE_API void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = 0) override;
        NEXUS_ENGINE_API void Free(void* Pointer) override;
        NEXUS_ENGINE_API void Clear() override;

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size = 0, uint64 Alignement = 0) const override;
        NEXUS_ENGINE_API bool IsValidAddress(void* Pointer) const override;

        NEXUS_ENGINE_API uint64 SlotAvailable() const;

    private:
        uint64* Head;
        uint64 Stride;
    };
}

