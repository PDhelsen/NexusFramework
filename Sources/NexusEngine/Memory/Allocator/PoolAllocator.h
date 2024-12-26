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
		NEXUS_ENGINE_API PoolAllocator(const PoolAllocator& Other) = delete;
		NEXUS_ENGINE_API PoolAllocator(PoolAllocator&& Other) noexcept = delete;
		NEXUS_ENGINE_API virtual ~PoolAllocator();

		NEXUS_ENGINE_API PoolAllocator& operator=(const PoolAllocator& Other) = delete;
		NEXUS_ENGINE_API PoolAllocator& operator=(PoolAllocator&& Other) noexcept = delete;

		NEXUS_ENGINE_API void Clear() override;
		NEXUS_ENGINE_API bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		NEXUS_ENGINE_API bool BelongToAllocator(void* Pointer) const override;

		NEXUS_ENGINE_API uint64 SlotAvailable() const { return FreeAmount() / Stride; }

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

    private:
		inline void Next();
		inline void Previous(void* Pointer);
		inline void Reset();

        uint64* Head;
        uint64 Stride;
    };
}

