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
	class Array
	{
	public:
		using I = Iterator::IteratorBlock<T>;

		template<typename... Args>
		Array(uint64 Size, Allocator* Allctr = nullptr, Args&&... args)
			: Alloc(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
			ConstructRange(0, Count, args...);
		}

		Array(const Array<T>& Other)
			: Alloc(Other.Alloc), Count(Other.Count), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Array - Copy constructor");

			Allocate(Count);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}
		}

		Array(Array<T>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Array()
		{
			DestructRange(0, Count);
			Free();
		}

		Array<T>& operator=(const Array<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Array - Assignement operator");

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

		Array<T>& operator=(Array<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			DestructRange(0, Count);
			Free();

			Alloc = Other.Alloc;
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

			Destruct(Index);
			Construct(Index, args...);
			return Data[Index];
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount()  - 1), "Invalid Index");

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
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
			Sort::SortIndexBased<T, S, Array<T>>(*this, Count, Function);
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

		uint64 GetCount() const { return Count; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Allocate(sizeof(T) * Count, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
			Data = nullptr;
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			Memory::Construct<T>(&Data[Index], args...);
		}

		template<typename... Args>
		void ConstructRange(uint64 Index, uint64 Size, Args&&... args)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Construct<T>(&Data[Index + Offset], args...);
			}
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

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		void ValidateCapacity(uint64 Size)
		{
			Count = Size > 1 ? Size : 1;
		}

		Allocator* Alloc;
		uint64 Count;
		T* Data;
	};
}
