#pragma once

#include "NexusFramework/Memory/Allocator/BucketAllocator.h"
#include "NexusFramework/Types/Containers/Dictionary.h"

namespace NxFr
{
	class NX_FRAMEWORK_API FixedAllocator : public BucketAllocator
	{
	public:
		NX_NOCOPY_NOMOVE(FixedAllocator)
		FixedAllocator(uint64 Size);
		virtual ~FixedAllocator();

		uint64 UsedAmount() const override;

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;

		Allocator* FindAllocator(uint64 Size, uint64 Alignement) override;
		Allocator* CreateAllocator(uint64 Size, uint64 Alignement) override;
		Allocator* GetAllocator(void* Pointer) const override;
		void ClearAllocators(bool Delete) override;
		void ResizeAllocation(uint64& Size);

		Dictionary<uint64, Allocator*> Allocators;
	};
}


