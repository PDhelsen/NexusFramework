#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/BucketAllocator.h"

namespace NxFr
{
	BucketAllocator::BucketAllocator(uint64 Size)
		: Allocator(0), BucketSize(Size)
	{
	}

	BucketAllocator::~BucketAllocator()
	{
	}

	void BucketAllocator::Clear()
	{
		ClearAllocators(false);
	}

	bool BucketAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		// It will always be able to allocate since it request new memory when full
		return true;
	}

	bool BucketAllocator::BelongToAllocator(void* Pointer) const
	{
		return GetAllocator(Pointer) != nullptr;
	}

	void* BucketAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		NX_ASSERT(Size < BucketSize, Default, "Allocation size requested overflow allocator size");

		Allocator* Alloc = FindOrCreateAllocator(Size, Alignement);

		uint64 Marker = Alloc->UsedAmount();

		void* Pointer = Memory::Allocate(Size, Alloc, Alignement);

		uint64 Delta = Alloc->UsedAmount() - Marker;
		IncreaseAmount(Delta);

		return Pointer;
	}

	void* BucketAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		if (!Pointer)
		{
			return nullptr;
		}

		NX_ASSERT(Size < BucketSize, Default, "Allocation size requested overflow allocator size");

		Allocator* Alloc = GetAllocator(Pointer);
		NX_ASSERT(Alloc, Default, "Memory was not allocated from this allocator");

		if (Alloc->CanAllocate(Size, Alignement))
		{
			uint64 Marker = Alloc->UsedAmount();

			Pointer = Memory::Reallocate(Pointer, Size, Alloc, Alignement);

			if (Alloc->UsedAmount() > Marker)
			{
				uint64 Delta = Alloc->UsedAmount() - Marker;
				IncreaseAmount(Delta);
			}
			else
			{
				uint64 Delta = Marker - Alloc->UsedAmount();
				DecreaseAmount(Delta);
			}
		}
		else
		{
			Free(Pointer);
			Pointer = Allocate(Size, Alignement);
		}

		return Pointer;
	}

	void BucketAllocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		Allocator* Alloc = GetAllocator(Pointer);
		NX_ASSERT(Alloc, Default, "Memory was not allocated from this allocator");

		uint64 Marker = Alloc->UsedAmount();

		Memory::Free(Pointer, Alloc);

		uint64 Delta = Marker - Alloc->UsedAmount();
		DecreaseAmount(Delta);
	}

	Allocator* BucketAllocator::FindOrCreateAllocator(uint64 Size, uint64 Alignement)
	{
		Allocator* Result = FindAllocator(Size, Alignement);
		if (Result == nullptr)
		{
			Result = CreateAllocator(Size, Alignement);
		}

		return Result;
	}
}
