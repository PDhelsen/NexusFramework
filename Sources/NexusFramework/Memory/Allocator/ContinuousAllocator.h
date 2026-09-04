#pragma once

#include "NexusFramework/Memory/Allocator/BucketAllocator.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Functions/Delegate.h"

namespace NxFr
{
	class NX_FRAMEWORK_API ContinuousAllocator : public BucketAllocator
	{
	public:
		template<typename T>
		static Delegate<Allocator*(uint64)> DefaultCreator() { return [](uint64 Bucket) { return new T(Bucket); }; }

		NX_NOCOPY_NOMOVE(ContinuousAllocator)
		ContinuousAllocator(uint64 Size, const Delegate<Allocator*(uint64)>& Creator);
		ContinuousAllocator(uint64 Size, Delegate<Allocator*(uint64)>&& Creator);
		virtual ~ContinuousAllocator();

		uint64 GetCount() const { return Allocators.GetCount(); }

	protected:
		Allocator* FindAllocator(uint64 Size, uint64 Alignement) override;
		Allocator* CreateAllocator(uint64 Size, uint64 Alignement) override;
		Allocator* GetAllocator(void* Pointer) const override;
		void ClearAllocators(bool Delete) override;

		Set<Allocator*> Allocators;
		Delegate<Allocator*(uint64)> Creator;
	};
}

