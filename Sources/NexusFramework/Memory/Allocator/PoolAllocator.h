#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
    class PoolAllocator : public MemoryAllocator
    {
    public:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, PoolAllocator)
		NEXUS_FRAMEWORK_API PoolAllocator(uint64 Size, uint64 Stride);
		NEXUS_FRAMEWORK_API virtual ~PoolAllocator();

		NEXUS_FRAMEWORK_API void Clear() override;
		NEXUS_FRAMEWORK_API bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		NEXUS_FRAMEWORK_API bool BelongToAllocator(void* Pointer) const override;

		NEXUS_FRAMEWORK_API uint64 SlotAvailable() const { return FreeAmount() / Stride; }
		NEXUS_FRAMEWORK_API uint64 GetStride() const { return Stride; }

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

    private:
		inline void Next();
		inline void Previous(void* Pointer);
		inline void Reset();

	private:
		uint64* Head;
        uint64 Stride;
    };
}

