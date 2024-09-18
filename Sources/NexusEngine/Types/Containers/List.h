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
	template<typename T>
	class List
	{
	public:
		using I = Iterator::IteratorBlock<T>;

		List(uint64 Size = DefaultSize, Allocator* Allctr = nullptr)
			: Alloc(nullptr), Capacity(0), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
		}

		List(const List<T>& Other)
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "List - Copy constructor");

			Allocate(Capacity);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}
		}

		List(List<T>&& Other) noexcept
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~List()
		{
			Clear();
			Free();
		}

		List<T>& operator=(const List<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "List - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Alloc = Other.Alloc;
			Capacity = Other.Capacity;
			Count = Other.Count;

			Allocate(Capacity);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}

			return *this;
		}

		List<T>& operator=(List<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Alloc = Other.Alloc;
			Capacity = Other.Capacity;
			Count = Other.Count;
			Data = Other.Data;

			Other.Capacity = 0;
			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
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

			Destruct(Index);
			Construct(Index, args...);
			return Data[Index];
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount() - 1), "Invalid Index");

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				Data[Index + Offset] = *It;
			}

			return Data[Index];
		}

		T& Append(const T& Value)
		{
			Resize(++Count);
			Construct(Count - 1, Value);
			return Data[Count - 1];
		}

		T& Append(T&& Value)
		{
			Resize(++Count);
			Construct(Count - 1, Move(Value));
			return Data[Count - 1];
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			Resize(++Count);
			Construct(Count - 1, args...);
			return Data[Count - 1];
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			uint64 Index = GetCount();
			Resize(GetCount() + Value.GetCount());

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				Construct(Index + Offset, *It);
			}
	
			return Data[Index];
		}

		T& Insert(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Construct(Index, Value);
			return Data[Index];
		}

		T& Insert(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Construct(Index, Move(Value));
			return Data[Index];
		}

		template<typename... Args>
		T& InsertConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Construct(Index, args...);
			return Data[Index];
		}

		template<typename C>
		T& InsertRange(uint64 Index, const C& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(GetCount() + Value.GetCount());
			Shift(Index, Value.GetCount(), true);

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				Construct(Index + Offset, *It);
			}
		
			return Data[Index];
		}

		void Remove(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Destruct(Index);
			Shift(Index, 1, false);
			Resize(--Count);
		}

		void RemoveRange(uint64 Index, uint64 Size)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Size - 1), "Invalid Index");

			DestructRange(Index, Size);
			Shift(Index, Size, false);
			Resize(Count - Size);
		}

		void RemoveLast()
		{
			NEXUS_ASSERT(IsValidIndex(Count - 1), "Invalid Index");
			
			Destruct(Count - 1);
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
			DestructRange(0, Count);
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

		I GetIterator(uint64 Index) const
		{
			return I(Data, Index);
		}

		I begin() const { return Begin(); }
		I Begin() const
		{
			return I(Data, 0);
		}

		I BeginReverse() const
		{
			return --End();
		}

		I end() const { return End(); }
		I End() const
		{
			return I(Data, Count);
		}

		I EndReverse() const
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
			for (uint64 Front = 0, Back = Count - 1; Front < Half; ++Front, --Back)
			{
				Swap(Front, Back);
			}
		}

		template<typename S = Sorting::DefaultIndexBased>
		void Sort(Sorting::CompareFunction<T> Function = nullptr)
		{
			Sort::SortIndexBased<T, S, List<T>>(*this, Count, Function);
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		I Find(const T& Other) const
		{
			for (I It = Begin(); It != End(); ++It)
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
			Data = (T*)Memory::Allocate(sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Alloc);
		}

		void Reallocate(uint64 Size)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "List - Reallocate");

			ValidateCapacity(Size);
			Data = (T*)Memory::Realloc(Data, sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			Memory::Construct<T>(&Data[Index], args...);
		}

		void Destruct(uint64 Index)
		{
			Memory::Destruct(&Data[Index]);
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Destruct(&Data[Index + Offset]);
			}
		}

		void Shift(uint64 Index, uint64 Size, bool Forward)
		{
			T* Start = Forward ? &Data[Index] : &Data[Index + Size];
			T* End = Forward ? &Data[Index + Size] : &Data[Index];
			Memory::MemMove(Start, End, sizeof(T) * (Count - Index - Size));
		}

		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				Reallocate(GrowPolicy());
			}
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		void ValidateCapacity(uint64 Size)
		{ 
			uint64 Lowest = Count > 2 ? Count : 2;
			Capacity = Size > Lowest ? Size : Lowest;
		}

		uint64 GrowPolicy() const
		{
			return Capacity * 2;
		}

		inline static const uint64 DefaultSize = 16;

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		T* Data;
	};
}
