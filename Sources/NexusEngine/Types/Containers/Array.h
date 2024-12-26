#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"
#include "Misc/Sort.h"

namespace NxEn
{
	template<typename T, uint64 L = 1>
	class Array
	{
	public:
		using I = Iterator::IteratorBlock<T>;

		Array()
			: Alloc(nullptr), Count(0)
		{
			NEXUS_ASSERT(L >= 1, "The provided size is invalid");

			Allocate(L);
			ConstructRange(0, Count);
		}

		Array(uint64 Size, Allocator* Allctr = nullptr)
			: Alloc(Allctr), Count(0)
		{
			NEXUS_ASSERT(L == 1 && Size > 1, "The provided size is invalid");

			Allocate(Size);
			ConstructRange(0, Count);
		}

		Array(const Array<T, L>& Other)
			: Alloc(Other.Alloc), Count(Other.Count)
		{
			NEXUS_LOG(Warning, LoggerChannel::Performance, "Array - Copy constructor");

			Allocate(Count);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}
		}

		Array(Array<T, L>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count)
		{
			if (!Other.IsStackArray())
			{
				Data.Heap = Other.Data.Heap;
				Other.Data.Heap = nullptr;
				Other.Count = 0;
			}
			else
			{
				for (uint64 Index = 0; Index < Count; ++Index)
				{
					Construct(Index, Other[Index]);
				}
			}
		}

		~Array()
		{
			DestructRange(0, Count);
			Free();
		}

		Array<T, L>& operator=(const Array<T, L>& Other)
		{
			NEXUS_LOG(Warning, LoggerChannel::Performance, "Array - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			DestructRange(0, Count);
			Free();

			Alloc = Other.Alloc;
			Count = Other.Count;

			Allocate(Count);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}

			return *this;
		}

		Array<T, L>& operator=(Array<T, L>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			DestructRange(0, Count);
			Free();

			Alloc = Other.Alloc;
			Count = Other.Count;

			if (!Other.IsStackArray())
			{
				Data.Heap = Other.Data.Heap;
				Other.Data.Heap = nullptr;
				Other.Count = 0;
			}
			else
			{
				for (uint64 Index = 0; Index < Count; ++Index)
				{
					Construct(Index, Other[Index]);
				}
			}

			return *this;
		}

		T& operator[](uint64 Index)
		{
			return Get(Index);
		}

		const T& operator[](uint64 Index) const
		{
			return Get(Index);
		}

		bool operator==(const Array<T, L>& Other) const
		{
			return Count == Other.Count && GetData() == Other.GetData();
		}

		bool operator!=(const Array<T, L>& Other) const
		{
			return !(*this == Other);
		}

		T& Assign(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			T& Item = GetItem(Index);
			Item = Value;
			return Item;
		}

		T& Assign(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			T& Item = GetItem(Index);
			Item = Move(Value);
			return Item;
		}

		template<typename... Args>
		T& AssignConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			T& Item = GetItem(Index);
			Destruct(Index);
			Construct(Index, args...);
			return Item;
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount()  - 1), "Invalid Index");

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				T& Item = GetItem(Index + Offset);
				Item = *It;
			}

			return GetItem(Index);
		}

		T& Get(uint64 Index) 
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			return GetItem(Index);
		}

		const T& Get(uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			return GetItem(Index);
		}

		T& First() 
		{
			return GetItem(0);
		}

		const T& First() const
		{
			return GetItem(0);
		}

		T& Last() 
		{
			return GetItem(Count - 1);
		}

		const T& Last() const
		{
			return GetItem(Count - 1);
		}

		I GetIterator(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			return GetIteratorIndex(Index);
		}

		const I GetIterator(uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			return GetIteratorIndex(Index);
		}

		I begin() { return Begin(); }
		I Begin() 
		{
			return GetIteratorIndex(0);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIteratorIndex(0);
		}

		I BeginReverse() 
		{
			return --End();
		}

		const I BeginReverse() const
		{
			return --End();
		}

		I end() { return End(); }
		I End() 
		{
			return GetIteratorIndex(Count);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIteratorIndex(Count);
		}

		I EndReverse()
		{
			return --Begin();
		}

		const I EndReverse() const
		{
			return --Begin();
		}

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Count;
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			NEXUS_ASSERT(IsValidIndex(IndexA), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(IndexB), "Invalid Index");

			T Temp = GetItem(IndexA);
			GetItem(IndexA) = Move(GetItem(IndexB));
			GetItem(IndexB) = Move(Temp);
		}

		void Reverse()
		{
			uint64 Half = Count / 2;
			for (uint64 Front = 0, Back = Count - 1; Front < Half; ++Front, --Back)
			{
				Swap(Front, Back);
			}
		}

		template<typename S = Sorting::DefaultIndexBased>
		void Sort(Sorting::CompareFunction<T> Function = nullptr)
		{
			Sort::SortIndexBased<T, S, Array<T, L>>(*this, Count, Function);
		}

		bool Contains(const T& Other) const
		{
			return GetIteratorValue(Other) != End();
		}

		I Find(const T& Other)
		{
			return GetIteratorValue(Other);
		}

		const I Find(const T& Other) const
		{
			return GetIteratorValue(Other);
		}

		uint64 GetCount() const { return Count; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			if (!IsStackArray())
			{
				Data.Heap = (T*)Memory::Allocate(sizeof(T) * Count, Alloc);
			}
		}

		void Free()
		{
			if (!IsStackArray())
			{
				Memory::Free(Data.Heap, Alloc);
				Data.Heap = nullptr;
			}
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			Memory::Construct<T>(&GetData()[Index], args...);
		}

		void ConstructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Construct<T>(&GetData()[Index + Offset]);
			}
		}

		void Destruct(uint64 Index)
		{
			Memory::Destruct(&GetData()[Index]);
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Destruct(&GetData()[Index + Offset]);
			}
		}

		bool IsStackArray() const
		{
			return L == Count;
		}

		T* GetData()
		{
			return IsStackArray() ? Data.Stack : Data.Heap;
		}

		const T* GetData() const
		{
			return IsStackArray() ? Data.Stack : Data.Heap;
		}

		T& GetItem(uint64 Index)
		{
			return GetData()[Index];
		}

		const T& GetItem(uint64 Index) const
		{
			return GetData()[Index];
		}

		I GetIteratorIndex(uint64 Index)
		{
			return I(GetData(), Index);
		}

		const I GetIteratorIndex(uint64 Index) const
		{
			return I(const_cast<T*>(GetData()), Index);
		}

		I GetIteratorValue(const T& Value) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (*It == Value)
				{
					return It;
				}
			}

			return End();
		}

		void ValidateCapacity(uint64 Size)
		{
			Count = Size > 1 ? Size : 1;
		}

		union Buffer
		{
			T* Heap;
			T Stack[L];

			Buffer(): Heap(nullptr) {};
			~Buffer() {};
		};

		Allocator* Alloc;
		uint64 Count;
		Buffer Data;
	};
}
