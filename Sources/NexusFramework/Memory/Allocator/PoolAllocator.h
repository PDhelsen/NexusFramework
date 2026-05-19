#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
    class NX_FRAMEWORK_API PoolAllocator : public MemoryAllocator
    {
    public:
		NX_NOCOPY_NOMOVE(PoolAllocator)
		PoolAllocator(uint64 Size, uint64 Stride);
		virtual ~PoolAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

		uint64 SlotAvailable() const { return FreeAmount() / Stride; }
		uint64 GetStride() const { return Stride; }

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

