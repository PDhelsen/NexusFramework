#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

#include "NexusFramework/Types/Containers/Set.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API RawAllocator : public Allocator
	{
	public:
		NEXUS_NOCOPY_NOMOVE(RawAllocator)
		RawAllocator();
		virtual ~RawAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

	private:
		Set<void*> Allocated;
	};
}
