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
			Iterator(T* Ptr)
				: Pointer(Ptr)
			{

			}

			Iterator& operator++()
			{
				Pointer++;
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
				Pointer--;
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
				return Pointer;
			}

			T& operator*() const
			{
				return *Pointer;
			}

			bool operator==(const Iterator& Other) const
			{
				return Pointer == Other.Pointer;
			}

			bool operator!=(const Iterator& Other) const
			{
				return Pointer != Other.Pointer;
			}

		private:
			T* Pointer;
		};

		List(uint64 Size = 2, Allocator* Allctr = nullptr)
			: Allocator(nullptr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
			Capacity = GetValidCapacity(Size);
			Data = (T*)Memory::Allocate(sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
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
			Memory::Free(Data, Allocator);
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
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			return Data[Index];
		}

		bool operator==(const List<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const List<T>& Other) const
		{
			return Count != Other.Count || Data != Other.Data;
		}

		void Assign(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			Data[Index] = Value;
		}

		void Assign(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			Data[Index] = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			Data[Index] = T(args...);
		}

		void AssignRange(uint64 Index, List<T>& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount() - 1), "Invalid Index");

			for (uint64 Offset = 0; Offset < Value.GetCount(); Offset++)
			{
				Data[Index + Offset] = Value[Offset];
			}
		}

		void Append(const T& Value)
		{
			Resize(++Count);
			Data[Count - 1] = Value;
		}

		void Append(T&& Value)
		{
			Resize(++Count);
			Data[Count - 1] = Move(Value);
		}

		template<typename... Args>
		void AppendConstruct(Args&&... args)
		{
			Resize(++Count);
			Data[Count - 1] = T(args...);
		}

		void AppendRange(const List<T>& Value)
		{
			uint64 Index = GetCount();
			Resize(GetCount() + Value.GetCount());
			for (uint64 Offset = 0; Offset < Value.GetCount(); Offset++)
			{
				Data[Index + Offset] = Value[Offset];
			}
		}

		void Insert(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Memory::MemCopy(&Data[Index], &Data[Index + 1], sizeof(T) * (Count - Index - 1));
			Data[Index] = Value;
		}

		void Insert(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Memory::MemCopy(&Data[Index], &Data[Index + 1], sizeof(T) * (Count - Index - 1));
			Data[Index] = Move(Value);
		}

		template<typename... Args>
		void InsertConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(++Count);
			Memory::MemCopy(&Data[Index], &Data[Index + 1], sizeof(T) * (Count - Index - 1));
			Data[Index] = T(args...);
		}

		void InsertRange(uint64 Index, const List<T>& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			Resize(GetCount() + Value.GetCount());
			Memory::MemCopy(&Data[Index], &Data[Index + Value.GetCount()], sizeof(T) * (Count - Index - Value.GetCount()));
			for (uint64 Offset = 0; Offset < Value.GetCount(); Offset++)
			{
				Data[Index + Offset] = Value[Offset];
			}
		}

		void Remove(uint64 Index)
		{
			Memory::MemCopy(&Data[Index + 1], &Data[Index], sizeof(T) * (Count - Index - 1));
			Resize(--Count);
		}

		void RemoveRange(uint64 Index, uint64 Size)
		{
			Memory::MemCopy(&Data[Index + Size], &Data[Index], sizeof(T) * (Count - Index - Size));
			Resize(Count - Size);
		}

		void RemoveLast()
		{
			Resize(--Count);
		}

		void RemoveSwap(uint64 Index)
		{
			Swap(Index, Count - 1);
			RemoveLast();
		}

		void Clear(bool ShrinkToZero = false)
		{
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
			return Data[0];
		}

		T& Last() const
		{
			return Data[Count - 1];
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data);
		}

		Iterator BeginReverse() const
		{
			return --End();
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data + Count);
		}

		Iterator EndReverse() const
		{
			return --Begin();
		}

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Count;
		}

		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				uint64 NewCapacity = Capacity + Capacity / 2;
				Capacity = GetValidCapacity(NewCapacity > Count ? NewCapacity : Count);

				Data = (T*)Memory::Realloc(Data, sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
			}

			NEXUS_ASSERT(Count <= Capacity, "Overflowing list");
		}

		void Reserve(uint64 Size)
		{
			if (Size <= Capacity)
			{
				return;
			}

			Capacity = GetValidCapacity(Size);
			Data = (T*)Memory::Realloc(Data, sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Shrink()
		{
			Capacity = GetValidCapacity(Count);
			Data = (T*)Memory::Realloc(Data, sizeof(T) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			NEXUS_ASSERT(IsValidIndex(IndexA), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(IndexB), "Invalid Index");

			T Temp = Data[IndexB];
			Data[IndexB] = Move(Data[IndexA]);
			Data[IndexA] = Move(Temp);
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) < Count;
		}

		uint64 Find(const T& Other) const
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				if (Data[Index] == Other)
				{
					return Index;
				}
			}

			return Count;
		}

		void Sort()
		{
			Sort::HeapSort(Data, Count);
		}

		void Reverse()
		{
			uint64 Half = Count / 2;
			for (uint64 Front = 0, Back = Count - 1; Front < Half; Front++, Back--)
			{
				Swap(Front, Back);
			}
		}

		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }
		bool IsEmpty() const { return Count == 0; }

	private:
		uint64 GetValidCapacity(uint64 Size) const { return Size > 2 ? Size : 2; }

		Allocator* Allocator;
		uint64 Capacity;
		uint64 Count;
		T* Data;
	};
}