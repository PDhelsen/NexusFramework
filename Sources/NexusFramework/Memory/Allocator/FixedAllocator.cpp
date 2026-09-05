#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/FixedAllocator.h"
#include "NexusFramework/Memory/Allocator/ContinuousAllocator.h"
#include "NexusFramework/Memory/Allocator/PoolAllocator.h"

namespace NxFr
{
	FixedAllocator::FixedAllocator(uint64 Size)
		: BucketAllocator(Size), Allocators()
	{
	}

	FixedAllocator::~FixedAllocator()
	{
		ClearAllocators(true);
	}

	uint64 FixedAllocator::UsedAmount() const
	{
		uint64 Amount = 0;
		for (auto [Stride, Allctr] : Allocators)
		{
			Amount += Allctr->UsedAmount();
		}
		return Amount;
	}

	void* FixedAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		ResizeAllocation(Size);
		return BucketAllocator::Allocate(Size, Alignement);
	}

	void* FixedAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		ResizeAllocation(Size);
		return BucketAllocator::Reallocate(Pointer, Size, Alignement);
	}

	Allocator* FixedAllocator::FindAllocator(uint64 Size, uint64 Alignement)
	{
		Allocator** Allctr = Allocators.TryGet(Size);
		return Allctr ? *Allctr : nullptr;
	}

	Allocator* FixedAllocator::CreateAllocator(uint64 Size, uint64 Alignement)
	{
		Allocator::Scope Context(nullptr);

		uint64 Stride = Size;
		uint64 PoolSize = Math::RoundToInt(BucketSize / (float)Stride) * Stride;

		Allocator* Allctr = new ContinuousAllocator(PoolSize, [Stride](uint64 BucketSize) { return new PoolAllocator(BucketSize, Stride); });
		Allocators.Append(Size, Allctr);

		return Allctr;
	}

	Allocator* FixedAllocator::GetAllocator(void* Pointer) const
	{
		for (auto [Size, Allctr] : Allocators)
		{
			if (Allctr->BelongToAllocator(Pointer))
			{
				return Allctr;
			}
		}

		return nullptr;
	}

	void FixedAllocator::ClearAllocators(bool Delete)
	{
		Allocator::Scope Context(nullptr);

		for (auto [Size, Allctr] : Allocators)
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

	void FixedAllocator::ResizeAllocation(uint64& Size)
	{
		Size = !Math::IsPowerOfTwo(Size) ? Math::NextPowerOfTwo(Size) : Size;
	}
}
