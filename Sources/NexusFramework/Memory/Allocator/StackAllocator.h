#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
    class NEXUS_FRAMEWORK_API StackAllocator : public MemoryAllocator
    {
    public:
		NEXUS_NOCOPY_NOMOVE(StackAllocator)
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
		inline void Next(void* Pointer);
		inline void Previous(void* Pointer);
		inline void Reset();

		void* Marker;
    };
}

