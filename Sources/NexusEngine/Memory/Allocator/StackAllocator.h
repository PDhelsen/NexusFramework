#pragma once

#include "Core/NexusEngineCore.h"
#include "Allocator.h"

namespace NxEn
{
    // Stack allocator
    // Return aligned pointer from pre-allocated memory
    // Last object allocated need to be the first de-allocated

    class StackAllocator : public Allocator
    {
    public:
        NEXUS_ENGINE_API StackAllocator(uint64 Size);
        NEXUS_ENGINE_API ~StackAllocator();

        NEXUS_ENGINE_API void* Allocate(uint64 Size, uint64 Alignement);
        NEXUS_ENGINE_API void Free(void* Pointer);
        NEXUS_ENGINE_API void Clear();

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size, uint64 Alignement) const;
        NEXUS_ENGINE_API bool IsValidAddress(void* Pointer) const;

    private:
        void* Marker;
    };
}

