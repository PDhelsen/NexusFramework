#pragma once

#include "NexusAllocator.h"

namespace NxEn
{
    // Pool allocator
    // Allow to store object from same size into a pool (doesn't need to be the same object type)
    // Implicit alignement since everything has the same size

    class PoolAllocator : public NexusAllocator
    {
    public:
        NEXUS_ENGINE_API PoolAllocator(uint64 Count, uint64 Stride);
		NEXUS_ENGINE_API PoolAllocator(const PoolAllocator& Other) = delete;
		NEXUS_ENGINE_API PoolAllocator(PoolAllocator&& Other) noexcept = delete;
		NEXUS_ENGINE_API virtual ~PoolAllocator();

		NEXUS_ENGINE_API PoolAllocator& operator=(const PoolAllocator& Other) = delete;
		NEXUS_ENGINE_API PoolAllocator& operator=(PoolAllocator&& Other) noexcept = delete;

        NEXUS_ENGINE_API void* Allocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) override;
        NEXUS_ENGINE_API void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) override;
        NEXUS_ENGINE_API void Free(void* Pointer) override;
        NEXUS_ENGINE_API void Clear() override;

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) const override;
        NEXUS_ENGINE_API bool IsAllocatedAddress(void* Pointer) const override;

        NEXUS_ENGINE_API uint64 SlotAvailable() const;

    private:
		void Next();
		void Previous(void* Pointer);
		void Reset();

        uint64* Head;
        uint64 Stride;
    };
}

