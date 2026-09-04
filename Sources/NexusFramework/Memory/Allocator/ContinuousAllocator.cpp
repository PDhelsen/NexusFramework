#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/ContinuousAllocator.h"

namespace NxFr
{
	ContinuousAllocator::ContinuousAllocator(uint64 Size, const Delegate<Allocator*(uint64)>& Creator)
		: BucketAllocator(Size), Allocators(), Creator(Creator)
	{
	}

	ContinuousAllocator::ContinuousAllocator(uint64 Size, Delegate<Allocator*(uint64)>&& Creator)
		: BucketAllocator(Size), Allocators(), Creator(Move(Creator))
	{
	}

	ContinuousAllocator::~ContinuousAllocator()
	{
		ClearAllocators(true);
	}

	Allocator* ContinuousAllocator::FindAllocator(uint64 Size, uint64 Alignement)
	{
		for (Allocator* Alloc : Allocators)
		{
			if (Alloc->CanAllocate(Size, Alignement))
			{
				return Alloc;
			}
		}

		return nullptr;
	}

	Allocator* ContinuousAllocator::CreateAllocator(uint64 Size, uint64 Alignement)
	{
		Allocator::Scope Context(nullptr);

		Allocator* Alloc = Creator.Invoke(BucketSize);
		Allocators.Append(Alloc);

		IncreaseAmount(Alloc->UsedAmount());
		return Alloc;
	}

	Allocator* ContinuousAllocator::GetAllocator(void* Pointer) const
	{
		for (Allocator* Alloc : Allocators)
		{
			if (Alloc->BelongToAllocator(Pointer))
			{
				return Alloc;
			}
		}

		return nullptr;
	}

	void ContinuousAllocator::ClearAllocators(bool Delete)
	{
		Allocator::Scope Context(nullptr);
		ResetAmount();

		for (Allocator* Alloc : Allocators)
		{
			Alloc->Clear();
			IncreaseAmount(Alloc->UsedAmount());

			if (Delete)
			{
				delete Alloc;
			}
		}

		if (Delete)
		{
			Allocators.Clear();
		}
	}
}
