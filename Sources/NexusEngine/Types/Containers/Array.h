#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/Sort.h"
#include "Misc/References.h"

namespace NxEn
{
	// TODO: Cleanup - Name - Allocator

	template<typename T>
	class Array
	{
	public:
		using Iterator = IteratorBlock<T>;

		template<typename... Args>
		Array(uint64 Size, Allocator* Allctr = nullptr, Args&&... args)
			: Allocator(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
			ConstructRange(0, Count, args...);
		}

		Array(const Array<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Array - Copy constructor");

			Allocate(Count);

			for (uint64 Index = 0; Index < Count; Index++)
			{
				Construct(Index, Other[Index]);
			}
		}

		Array(Array<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Array()
		{
			Destruct(0, Count);
			Free();
		}

		Array<T>& operator=(const Array<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Array - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Destruct(0, Count);
			Free();

			Allocator = Other.Allocator;
			Count = Other.Count;

			Allocate(Count);

			for (uint64 Index = 0; Index < Count; Index++)
			{
				Construct(Index, Other[Index]);
			}

			return *this;
		}

		Array<T>& operator=(Array<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Destruct(0, Count);
			Free();

			Allocator = Other.Allocator;
			Count = Other.Count;
			Data = Other.Data;

			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
		}

		T& operator[](uint64 Index) const
		{
			return Get(Index);
		}

		bool operator==(const Array<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Array<T>& Other) const
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

			Destruct(Index, 1);
			Construct(Index, args...);
			return Data[Index];
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount()  - 1), "Invalid Index");

			uint64 Offset = 0;
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++, Offset++)
			{
				Data[Index + Offset] = *It;
			}

			return Data[Index];
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

		void Sort(Sort::CompareFunction<T> Function = nullptr)
		{
			Sort::HeapSort<T>(*this, Function);
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

		uint64 GetCount() const { return Count; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Allocate(sizeof(T) * Count, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Free()
		{
			Memory::Free(Data, Allocator);
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			Memory::Construct<T>(&Data[Index], args...);
		}

		template<typename... Args>
		void ConstructRange(uint64 Index, uint64 Size, Args&&... args)
		{
			for (uint64 Offset = 0; Offset < Size; Offset++)
			{
				Memory::Construct<T>(&Data[Index + Offset], args...);
			}
		}

		void Destruct(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; Offset++)
			{
				Memory::Destruct(&Data[Index + Offset]);
			}
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		void ValidateCapacity(uint64 Size)
		{
			Count = Size > 1 ? Size : 1;
		}

		Allocator* Allocator;
		uint64 Count;
		T* Data;
	};
}
