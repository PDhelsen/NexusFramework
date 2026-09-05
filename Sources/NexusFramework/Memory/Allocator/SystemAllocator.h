#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

#include "NexusFramework/Types/Containers/Set.h"

namespace NxFr
{
	class NX_FRAMEWORK_API SystemAllocator : public Allocator
	{
	public:
		NX_NOCOPY_NOMOVE(SystemAllocator)
		SystemAllocator();
		virtual ~SystemAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

		uint64 UsedAmount() const override { return 0; }
		uint64 FreeAmount() const override { return 0; }
		uint64 TotalAmount() const override { return 0; }
		float Usage() const override { return -1.0f; }
		bool IsFull() const override { return false; }
		bool IsEmpty() const override { return false; }

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

	private:
		Set<void*> Allocated;
	};
}
