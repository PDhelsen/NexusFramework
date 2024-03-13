#pragma once

#include "Core/NexusEngineCore.h"
#include "Allocator.h"

namespace NxEn
{
    class StackAllocator : public Allocator
    {
    public:
        NEXUS_ENGINE_API StackAllocator(uint64 Size);
        NEXUS_ENGINE_API ~StackAllocator();

        NEXUS_ENGINE_API virtual void* Allocate(uint64 Size, uint64 Alignement);
        NEXUS_ENGINE_API virtual void Free(void* Pointer);
        NEXUS_ENGINE_API virtual void Clear();

        NEXUS_ENGINE_API virtual bool CanFit(uint64 Size, uint64 Alignement) const;
        NEXUS_ENGINE_API virtual bool ValidAddress(void* Pointer) const;

    private:
        void UpdateAmount();

        void* Marker;
    };
}

