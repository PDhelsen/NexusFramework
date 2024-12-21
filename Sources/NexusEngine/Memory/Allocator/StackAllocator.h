#pragma once

#include "NexusAllocator.h"

namespace NxEn
{
    // Stack allocator
    // Return aligned pointer from pre-allocated memory
    // Last object allocated need to be the first de-allocated

    class StackAllocator : public NexusAllocator
    {
    public:
        NEXUS_ENGINE_API StackAllocator(uint64 Size);
		NEXUS_ENGINE_API StackAllocator(const StackAllocator& Other) = delete;
		NEXUS_ENGINE_API StackAllocator(StackAllocator&& Other) noexcept = delete;
        NEXUS_ENGINE_API virtual ~StackAllocator();

		NEXUS_ENGINE_API StackAllocator& operator=(const StackAllocator& Other) = delete;
		NEXUS_ENGINE_API StackAllocator& operator=(StackAllocator&& Other) noexcept = delete;

        NEXUS_ENGINE_API void* Allocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) override;
        NEXUS_ENGINE_API void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) override;
        NEXUS_ENGINE_API void Free(void* Pointer) override;
        NEXUS_ENGINE_API void Clear() override;

        NEXUS_ENGINE_API bool CanAllocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) const override;
        NEXUS_ENGINE_API bool IsAllocatedAddress(void* Pointer) const override;

    private:
		inline void* NextPointer(void* Pointer, uint64 Size, uint64 Alignement) const;
		inline bool IsPointerFree(void* Pointer) const;

        void* Marker;
    };
}

