#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Memory/Memory.h"

namespace NxEn
{
	// Base class for custom allocator

	class Allocator;

	struct AllocatorActive
	{
	public:
		NEXUS_ENGINE_API AllocatorActive(Allocator* Allocator);
		NEXUS_ENGINE_API ~AllocatorActive();
	};

	class Allocator
	{
	public:
		NEXUS_ENGINE_API virtual void* Allocate(uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) = 0;
		NEXUS_ENGINE_API virtual void* Reallocate(void* Pointer, uint64 Size = 0, uint64 Alignement = NEXUS_MEMORY_ALIGN) = 0;
		NEXUS_ENGINE_API virtual void Free(void* Pointer) = 0;

	protected:
		Allocator() = default;
		Allocator(const Allocator& Other) = delete;
		Allocator(Allocator&& Other) noexcept = delete;
		virtual ~Allocator() = default;

		Allocator& operator=(const Allocator& Other) = delete;
		Allocator& operator=(Allocator&& Other) noexcept = delete;
	};
}

