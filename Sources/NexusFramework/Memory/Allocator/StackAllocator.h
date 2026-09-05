#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
    class NX_FRAMEWORK_API StackAllocator : public MemoryAllocator
    {
    public:
		NX_NOCOPY_NOMOVE(StackAllocator)
        StackAllocator(uint64 Size);
        virtual ~StackAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

    private:
		void* Marker;
    };
}

