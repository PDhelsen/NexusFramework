#pragma once

#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Iterator/IteratorAny.h"

namespace NxFr
{
	template<typename T>
	class Collection
	{
	public:
		using I = Iterator::IteratorAny<T>;

		template<typename C>
		Collection(const C& Container)
			: IteratorBegin(Container.Begin()), IteratorEnd(Container.End()),Count(Container.GetCount())
		{
		}

		Collection(const Collection<T>& Other)
			: IteratorBegin(Other.IteratorBegin), IteratorEnd(Other.IteratorEnd), Count(Other.Count)
		{
		}

		Collection(Collection<T>&& Other) noexcept
			: IteratorBegin(Other.IteratorBegin), IteratorEnd(Other.IteratorEnd), Count(Other.Count)
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

			IteratorBegin = Other.IteratorBegin;
			IteratorEnd = Other.IteratorEnd;
			Count = Other.Count;
		}

		Collection<T>& operator=(Collection<T>&& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			IteratorBegin = Other.IteratorBegin;
			IteratorEnd = Other.IteratorEnd;
			Count = Other.Count;
		}

		I begin() { return Begin(); }
		I Begin() { return IteratorBegin; }
		const I begin() const { return Begin(); }
		const I Begin() const { return IteratorBegin; }
		I end() { return End(); }
		I End() { return IteratorEnd; }
		const I end() const { return End(); }
		const I End() const { return IteratorEnd; }

		uint64 GetCount() const { return Count; }

	private:
		I IteratorBegin;
		I IteratorEnd;
		uint64 Count;
	};
}
