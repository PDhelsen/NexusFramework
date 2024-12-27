#pragma once

#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/AllocatorContext.h"

namespace NxEn
{
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

			T* operator->()
			{
				return &Get();
			}

			const T* operator->() const
			{
				return &Get();
			}

			T& operator*()
			{
				return Get();
			}

			const T& operator*() const
			{
				return Get();
			}

			bool operator==(const Interface<T>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const Interface<T>& Other) const
			{
				return !Equals(Other);
			}

			virtual bool Equals(const Interface<T>& Other) const = 0;
			virtual T& Get() = 0;
			virtual const T& Get() const = 0;
			virtual void Iterate() = 0;
			virtual void Copy(const Interface<T>& Other) = 0;
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

			T& Get() override
			{
				return Iterator.Get();
			}

			const T& Get() const override
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
			Alloc = AllocatorContext::Get();
			IteratorCurrent = new Wrapper<T, typename C::I>(Container.Begin());
			IteratorBegin = new Wrapper<T, typename C::I>(Container.Begin());
			IteratorEnd = new Wrapper<T, typename C::I>(Container.End());
			Count = Container.GetCount();
		}

		Collection(const Collection<T>& Other) = delete;
		Collection(Collection<T>&& Other) noexcept = delete;
		
		~Collection()
		{
			AllocatorContext Context(Alloc);
			delete IteratorCurrent;
			delete IteratorBegin;
			delete IteratorEnd;
		}

		Collection<T>& operator=(const Collection<T>& Other) = delete;
		Collection<T>& operator=(Collection<T>&& Other) noexcept = delete;

		Iterator& Reset() const { IteratorCurrent->Copy(*IteratorBegin); return *IteratorCurrent; }
		Iterator& Current() const { return *IteratorCurrent; }
		const Iterator& Begin() const { return *IteratorBegin; }
		const Iterator& End() const { return *IteratorEnd; }
		uint64 GetCount() const { return Count; }

	private:
		Allocator* Alloc;
		Iterator* IteratorCurrent;
		const Iterator* IteratorBegin;
		const Iterator* IteratorEnd;
		uint64 Count;
	};
}
