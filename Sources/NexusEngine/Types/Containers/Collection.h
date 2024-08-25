#pragma once

#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"

namespace NxEn
{
	// FIXME: Leak - Memory - Explicit Copy / Move Constructor / Operator

	template<typename T>
	class Collection
	{
	private:
		template<typename T>
		class Interface
		{
		public:
			Interface& operator++()
			{
				Iterate();
				return *this;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const Interface& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const Interface& Other) const
			{
				return !Equals(Other);
			}

			virtual bool Equals(const Interface& Other) const = 0;
			virtual T& Get() const = 0;
			virtual void Iterate() = 0;
			virtual void Copy(const Interface& Other) = 0;
		};

		template<typename T, typename I>
		class Wrapper : public Interface<T>
		{
		public:
			Wrapper(const I& It)
				: Iterator(It)
			{
			}

			bool Equals(const Interface<T>& Other) const override
			{
				const Wrapper* Cast = dynamic_cast<const Wrapper*>(&Other);
				NEXUS_ASSERT(Cast, "Failed to cast");
				return Iterator.Equals(Cast->Iterator);
			}

			T& Get() const override
			{
				return Iterator.Get();
			}

			void Iterate() override
			{
				return Iterator.Iterate();
			}

			void Copy(const Interface<T>& Other) override
			{
				const Wrapper* Cast = dynamic_cast<const Wrapper*>(&Other);
				NEXUS_ASSERT(Cast, "Failed to cast");
				Iterator = Cast->Iterator;
			}

		private:
			I Iterator;
		};

	public:
		using Iterator = Interface<T>;

		template<typename C>
		Collection(const C& Container)
		{
			Alloc = Memory::GetActiveAllocator();
			IteratorCurrent = new Wrapper<T, typename C::Iterator>(Container.Begin());
			IteratorBegin = new Wrapper<T, typename C::Iterator>(Container.Begin());
			IteratorEnd = new Wrapper<T, typename C::Iterator>(Container.End());
		}

		~Collection()
		{
			AllocatorActive Active(Alloc);
			delete IteratorCurrent;
			delete IteratorBegin;
			delete IteratorEnd;
		}

		Iterator& Reset() { IteratorCurrent->Copy(*IteratorBegin); return *IteratorCurrent; }
		Iterator& Current() const { return *IteratorCurrent; }
		const Iterator& Begin() const { return *IteratorBegin; }
		const Iterator& End() const { return *IteratorEnd; }

	private:
		Allocator* Alloc;
		Iterator* IteratorCurrent;
		const Iterator* IteratorBegin;
		const Iterator* IteratorEnd;
	};
}
