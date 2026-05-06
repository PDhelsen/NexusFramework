#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
    class StackAllocator : public MemoryAllocator
    {
    public:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, StackAllocator)
        NEXUS_FRAMEWORK_API StackAllocator(uint64 Size);
        NEXUS_FRAMEWORK_API virtual ~StackAllocator();

		NEXUS_FRAMEWORK_API void Clear() override;
		NEXUS_FRAMEWORK_API bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		NEXUS_FRAMEWORK_API bool BelongToAllocator(void* Pointer) const override;

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

    private:
		inline void Next(void* Pointer);
		inline void Previous(void* Pointer);
		inline void Reset();

	private:
		void* Marker;
    };
}

