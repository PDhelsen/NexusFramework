#pragma once

#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"

namespace NxFr
{
	template<typename T>
	class Collection
	{
	public:
		using I = Iterator::IteratorAbstract<T>;

		template<typename C>
		Collection(const C& Container)
			: IteratorCurrent(Container.Begin()), IteratorBegin(Container.Begin()), IteratorEnd(Container.End()),Count(Container.GetCount())
		{
		}

		Collection(const Collection<T>& Other)
			: IteratorCurrent(Other.IteratorCurrent), IteratorBegin(Other.IteratorBegin), IteratorEnd(Other.IteratorEnd), Count(Other.Count)
		{
		}
		
		~Collection()
		{
		}

		Collection<T>& operator=(const Collection<T>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			IteratorCurrent.Clone(Other.IteratorCurrent);
			IteratorBegin.Clone(Other.IteratorBegin);
			IteratorEnd.Clone(Other.IteratorEnd);
			Count = Other.Count;
		}

		I& Reset() const { IteratorCurrent.Clone(IteratorBegin); return IteratorCurrent; }
		I& Current() const { return IteratorCurrent; }
		const I& Begin() const { return IteratorBegin; }
		const I& End() const { return IteratorEnd; }

		uint64 GetCount() const { return Count; }

	private:
		mutable I IteratorCurrent;
		I IteratorBegin;
		I IteratorEnd;
		uint64 Count;
	};
}
