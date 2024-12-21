#pragma once

#include "Allocator.h"

namespace NxEn
{
	class GlobalAllocator : public Allocator
	{
	public:
		NEXUS_ENGINE_API GlobalAllocator() = default;
		NEXUS_ENGINE_API GlobalAllocator(const GlobalAllocator& Other) = delete;
		NEXUS_ENGINE_API GlobalAllocator(GlobalAllocator&& Other) noexcept = delete;
		NEXUS_ENGINE_API virtual ~GlobalAllocator() = default;

		NEXUS_ENGINE_API GlobalAllocator& operator=(const GlobalAllocator& Other) = delete;
		NEXUS_ENGINE_API GlobalAllocator& operator=(GlobalAllocator&& Other) noexcept = delete;

		NEXUS_ENGINE_API void* Allocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) override;
		NEXUS_ENGINE_API void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) override;
		NEXUS_ENGINE_API void Free(void* Pointer) override;
	};
}
