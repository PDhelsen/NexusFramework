#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/ContinuousAllocator.h"

namespace NxFr
{
	ContinuousAllocator::ContinuousAllocator(uint64 BucketSize, const Delegate<Allocator*(uint64)>& Creator)
		: BucketAllocator(BucketSize), Allocators(), Creator(Creator)
	{
	}

	ContinuousAllocator::ContinuousAllocator(uint64 BucketSize, Delegate<Allocator*(uint64)>&& Creator)
		: BucketAllocator(BucketSize), Allocators(), Creator(Move(Creator))
	{
	}

	ContinuousAllocator::~ContinuousAllocator()
	{
		ClearAllocators(true);
	}

	uint64 ContinuousAllocator::UsedAmount() const
	{
		uint64 Amount = 0;
		for (auto* Allctr : Allocators)
		{
			Amount += Allctr->UsedAmount();
		}
		return Amount;
	}

	Allocator* ContinuousAllocator::FindAllocator(uint64 Size, uint64 Alignement)
	{
		for (Allocator* Allctr : Allocators)
		{
			if (Allctr->CanAllocate(Size, Alignement))
			{
				return Allctr;
			}
		}

		return nullptr;
	}

	Allocator* ContinuousAllocator::CreateAllocator(uint64 Size, uint64 Alignement)
	{
		Allocator::Scope Context(nullptr);

		Allocator* Allctr = Creator.Invoke(BucketSize);
		Allocators.Append(Allctr);

		return Allctr;
	}

	Allocator* ContinuousAllocator::GetAllocator(void* Pointer) const
	{
		for (Allocator* Allctr : Allocators)
		{
			if (Allctr->BelongToAllocator(Pointer))
			{
				return Allctr;
			}
		}

		return nullptr;
	}

	void ContinuousAllocator::ClearAllocators(bool Delete)
	{
		Allocator::Scope Context(nullptr);

		for (Allocator* Allctr : Allocators)
		{
			Allctr->Clear();
			if (Delete)
			{
				delete Allctr;
			}
		}

		if (Delete)
		{
			Allocators.Clear();
		}
	}
}
