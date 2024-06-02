#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/Sort.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class List
	{
	public:
		class Iterator
		{
		public:
			Iterator(T* Pointer, uint64 Idx)
				: Data(Pointer), Index(Idx)
			{

			}

			Iterator& operator++()
			{
				Index++;
				return *this;
			}

			Iterator operator++(int32)
			{
				Iterator Temp = *this;
				++(*this);
				return Temp;
			}

			Iterator& operator--()
			{
				Index--;
				return *this;
			}

			Iterator operator--(int32)
			{
				Iterator Temp = *this;
				--(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Data[Index];
			}

			T& operator*() const
			{
				return Data[Index];
			}

			bool operator==(const Iterator& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			bool operator!=(const Iterator& Other) const
			{
				return !(*this == Other);
			}

			uint64 GetIndex() const { return Index; }

		private:
			T* Data;
			uint64 Index;
		};

		List(uint64 Size = 2, Allocator* Allctr = nullptr)
			: Allocator(nullptr), Capacity(0), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
		}

		List(const List<T>& Other)
			: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
		}

		List(List<T>&& Other) noexcept
			: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~List()
		{
			Clear();
			Free();
		}

		List<T> Copy() const
		{
			List<T> Copy = List<T>(Capacity, Allocator);
			Copy.Count = Count;

			for (uint64 Index = 0; Index < Count; Index++)
			{
				Copy.Data[Index] = Data[Index];
			}

			return Copy;
		}

		T& operator[](uint64 Index) const
		{
			return Get(Index);
		}

		bool operator==(const List<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const List<T>& Other) const
		{
			return !(*this == Other);
		}

		T& Assign(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Data[Index] = Value;
			return Data[Index];
		}

		T& Assign(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Data[Index] = Move(Value);
			return Data[Index];
		}

		template<typename... Args>
		T& AssignConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Memory::Construct<T>(&Data[Index], args...);
			return Data[Index];
		}

		T& AssignRange(uint64 Index, const List<T>& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount() - 1), "Invalid Index");

			uint64 Offset = 0;
			for (Iterator It = Value.Begin(); It != Value.End(); It++, Offset++)
			{
				Data[Index + Offset] = *It;
			}

			return Data[Index];
		}

		T& Append(const T& Value)
		{
			Resize(++Count);
			Data[Count - 1] = Value;
			return Data[Count - 1];
		}

		T& Append(T&& Value)
		{
			Resize(++Count);
			Data[Count - 1] = Move(Value);
			return Data[Count - 1];
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			Resize(++Count);
			Memory::Construct<T>(&Data[Count - 1], args...);
			return Data[Count - 1];
		}

		T& AppendRange(const List<T>& Value)
		{
			uint64 Index = GetCount();
			Resize(GetCount() + Value.GetCount());

			uint64 Offset = 0;
			for (Iterator It = Value.Begin(); It != Value.End(); It++, Offset++)
			{
				Data[Index + Offset] = *It;
			}
	
			return Data[Index];
		}

		T& Insert(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Data[Index] = Value;
			return Data[Index];
		}

		T& Insert(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Data[Index] = Move(Value);
			return Data[Index];
		}

		template<typename... Args>
		T& InsertConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Memory::Construct<T>(&Data[Index], args...);
			return Data[Index];
		}

		T& InsertRange(uint64 Index, const List<T>& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(GetCount() + Value.GetCount());
			Shift(Index, Value.GetCount(), true);

			uint64 Offset = 0;
			for (Iterator It = Value.Begin(); It != Value.End(); It++, Offset++)
			{
				Data[Index + Offset] = *It;
			}
		
			return Data[Index];
		}

		void Remove(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Destruct(Index, 1);
			Shift(Index, 1, false);
			Resize(--Count);
		}

		void RemoveRange(uint64 Index, uint64 Size)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Size - 1), "Invalid Index");

			Destruct(Index, Size);
			Shift(Index, Size, false);
			Resize(Count - Size);
		}

		void RemoveLast()
		{
			NEXUS_ASSERT(IsValidIndex(Count - 1), "Invalid Index");
			
			Destruct(Count - 1, 1);
			Resize(--Count);
		}

		void RemoveSwap(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Swap(Index, Count - 1);
			RemoveLast();
		}

		void Clear(bool ShrinkToZero = false)
		{
			Destruct(0, Count);
			Resize(0);
			if (ShrinkToZero)
			{
				Shrink();
			}
		}

		T& Get(uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			return Data[Index];
		}

		T& First() const
		{
			return Get(0);
		}

		T& Last() const
		{
			return Get(Count - 1);
		}

		Iterator GetIterator(uint64 Index) const
		{
			return Iterator(Data, Index);
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, 0);
		}

		Iterator BeginReverse() const
		{
			return --End();
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data, Count);
		}

		Iterator EndReverse() const
		{
			return --Begin();
		}

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Count;
		}

		void Grow(uint64 Size)
		{
			if (Size <= Capacity)
			{
				return;
			}

			Reallocate(Size);
		}

		void Shrink(uint64 Size = 0)
		{
			if (Size > Capacity)
			{
				return;
			}

			Reallocate(Size);
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			NEXUS_ASSERT(IsValidIndex(IndexA), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(IndexB), "Invalid Index");
			
			T Temp = Get(IndexA);
			Get(IndexA) = Move(Get(IndexB));
			Get(IndexB) = Move(Temp);
		}

		void Reverse()
		{
			uint64 Half = Count / 2;
			for (uint64 Front = 0, Back = Count - 1; Front < Half; Front++, Back--)
			{
				Swap(Front, Back);
			}
		}

		void Sort()
		{
			Sort::HeapSort(Data, Count);
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		Iterator Find(const T& Other) const
		{
			for (Iterator It = Begin(); It != End(); It++)
			{
				if (*It == Other)
				{
					return It;
				}
			}

			return End();
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Allocate(sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Reallocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Realloc(Data, sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Free()
		{
			Memory::Free(Data, Allocator);
		}

		void Destruct(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; Offset++)
			{
				Memory::Destruct(&Data[Index + Offset]);
			}
		}

		void Shift(uint64 Index, uint64 Size, bool Forward)
		{
			T* Start = Forward ? &Data[Index] : &Data[Index + Size];
			T* End = Forward ? &Data[Index + Size] : &Data[Index];
			Memory::MemCopy(Start, End, sizeof(T) * (Count - Index - Size));
		}

		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				Reallocate(Capacity + Capacity / 2);
			}
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		void ValidateCapacity(uint64 Size)
		{ 
			uint64 Lowest = Count > 2 ? Count : 2;
			Capacity = Size > Lowest ? Size : Lowest;
		}

		Allocator* Allocator;
		uint64 Capacity;
		uint64 Count;
		T* Data;
	};
}