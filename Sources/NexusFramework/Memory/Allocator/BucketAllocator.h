#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	class NX_FRAMEWORK_API BucketAllocator : public Allocator
	{
	public:
		NX_NOCOPY_NOMOVE(BucketAllocator)
		BucketAllocator(uint64 Size);
		virtual ~BucketAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

		uint64 GetBucketSize() const { return BucketSize; }
		void SetBucketSize(uint64 Size) { this->BucketSize = Size; }

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

		virtual Allocator* FindOrCreateAllocator(uint64 Size, uint64 Alignement);
		virtual Allocator* FindAllocator(uint64 Size, uint64 Alignement) = 0;
		virtual Allocator* CreateAllocator(uint64 Size, uint64 Alignement) = 0;
		virtual Allocator* GetAllocator(void* Pointer) const = 0;
		virtual void ClearAllocators(bool Delete) = 0;

		uint64 BucketSize;
	};
}
