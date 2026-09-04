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
		Allocator** Alloc = Allocators.TryGet(Size);
		return Alloc ? *Alloc : nullptr;
	}

	Allocator* FixedAllocator::CreateAllocator(uint64 Size, uint64 Alignement)
	{
		Allocator::Scope Context(nullptr);

		uint64 Stride = Size;
		uint64 PoolSize = Math::RoundToInt(BucketSize / (float)Stride) * Stride;

		Allocator* Alloc = new ContinuousAllocator(PoolSize, [Stride](uint64 BucketSize) { return new PoolAllocator(BucketSize, Stride); });
		Allocators.Append(Size, Alloc);

		return Alloc;
	}

	Allocator* FixedAllocator::GetAllocator(void* Pointer) const
	{
		for (auto [Size, Alloc] : Allocators)
		{
			if (Alloc->BelongToAllocator(Pointer))
			{
				return Alloc;
			}
		}

		return nullptr;
	}

	void FixedAllocator::ClearAllocators(bool Delete)
	{
		Allocator::Scope Context(nullptr);
		ResetAmount();

		for (auto [Size, Alloc] : Allocators)
		{
			Alloc->Clear();

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

	void FixedAllocator::ResizeAllocation(uint64& Size)
	{
		Size = !Math::IsPowerOfTwo(Size) ? Math::NextPowerOfTwo(Size) : Size;
	}
}
