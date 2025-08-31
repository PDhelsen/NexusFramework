#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Types/Containers/Set.h"

namespace NxFr
{
	class RawAllocator : public Allocator
	{
	public:
		NEXUS_FRAMEWORK_API RawAllocator();
		NEXUS_FRAMEWORK_API RawAllocator(const RawAllocator& Other) = delete;
		NEXUS_FRAMEWORK_API RawAllocator(RawAllocator&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API virtual ~RawAllocator();

		NEXUS_FRAMEWORK_API RawAllocator& operator=(const RawAllocator& Other) = delete;
		NEXUS_FRAMEWORK_API RawAllocator& operator=(RawAllocator&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Clear() override;
		NEXUS_FRAMEWORK_API bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		NEXUS_FRAMEWORK_API bool BelongToAllocator(void* Pointer) const override;

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

	private:
		Set<void*> Allocated;
	};
}
