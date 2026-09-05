#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Misc/Iterator.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename T>
	class List
	{
	public:
		using I = Iterator::Block<T>;

		inline static const uint64 DefaultSize = 8;

		List(uint64 Size = DefaultSize, Allocator * Allctr = Allocator::TryGet())
			: Allctr(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Size);
		}

		List(InitializerList<T> Init, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Init.size());

			for (auto& It : Init)
			{
				Append(It);
			}
		}

		List(const List<T>& Other)
			: Allctr(Other.Allctr), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			Allocate(Capacity);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}
		}

		List(List<T>&& Other) noexcept
			: Allctr(Other.Allctr), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Capacity = 0;
			Other.Count = 0;
			Other.Data = nullptr;
		}

		~List()
		{
			Clear();
			Free();
		}

		List<T>& operator=(const List<T>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();
			Free();

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
			if (this == &Other)
			{
				return *this;
			}

			Clear();
			Free();

			if (Allctr == Other.Allctr)
			{
				Capacity = Other.Capacity;
				Count = Other.Count;
				Data = Other.Data;

				Other.Capacity = 0;
				Other.Count = 0;
				Other.Data = nullptr;
			}
			else
			{
				Capacity = Other.Capacity;
				Count = Other.Count;

				Allocate(Capacity);

				for (uint64 Index = 0; Index < Count; ++Index)
				{
					Construct(Index, Move(Other[Index]));
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

		bool operator==(const List<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const List<T>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const List<T>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const List<T>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const List<T>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const List<T>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Assign(uint64 Index, const T& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			T& Item = GetItem(Index);
			Item = Value;
			return Item;
		}

		T& Assign(uint64 Index, T&& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			T& Item = GetItem(Index);
			Item = Move(Value);
			return Item;
		}

		template<typename... Args>
		T& AssignConstruct(uint64 Index, Args&&... args)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			T& Item = GetItem(Index);
			Destruct(Index);
			Construct(Index, args...);
			return Item;
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");
			NX_ASSERT(IsValidIndex(Index + Value.GetCount() - 1), Default, "Invalid Index");

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				T& Item = GetItem(Index + Offset);
				Item = *It;
			}

			return GetItem(Index);
		}

		T& Append(const T& Value)
		{
			Resize(++Count);
			Construct(Count - 1, Value);
			return GetItem(Count - 1);
		}

		T& Append(T&& Value)
		{
			Resize(++Count);
			Construct(Count - 1, Move(Value));
			return GetItem(Count - 1);
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			Resize(++Count);
			Construct(Count - 1, args...);
			return GetItem(Count - 1);
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
	
			return GetItem(Index);
		}

		T& Insert(uint64 Index, const T& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Construct(Index, Value);
			return GetItem(Index);
		}

		T& Insert(uint64 Index, T&& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Construct(Index, Move(Value));
			return GetItem(Index);
		}

		template<typename... Args>
		T& InsertConstruct(uint64 Index, Args&&... args)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			Resize(++Count);
			Shift(Index, 1, true);
			Construct(Index, args...);
			return GetItem(Index);
		}

		template<typename C>
		T& InsertRange(uint64 Index, const C& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			Resize(GetCount() + Value.GetCount());
			Shift(Index, Value.GetCount(), true);

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				Construct(Index + Offset, *It);
			}
		
			return GetItem(Index);
		}

		void Remove(uint64 Index)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			Destruct(Index);
			Shift(Index, 1, false);
			Resize(--Count);
		}

		void RemoveRange(uint64 Index, uint64 Size)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");
			NX_ASSERT(IsValidIndex(Index + Size - 1), Default, "Invalid Index");

			DestructRange(Index, Size);
			Shift(Index, Size, false);
			Resize(Count - Size);
		}

		void RemoveLast()
		{
			NX_ASSERT(IsValidIndex(Count - 1), Default, "Invalid Index");
			
			Destruct(Count - 1);
			Resize(--Count);
		}

		void RemoveSwap(uint64 Index)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			GetItem(Index) = GetItem(Count - 1);
			RemoveLast();
		}

		void Clear()
		{
			DestructRange(0, Count);
			Resize(0);
		}

		T& Get(uint64 Index)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetItem(Index);
		}

		const T& Get(uint64 Index) const
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetItem(Index);
		}

		T& First() 
		{
			NX_ASSERT(IsValidIndex(0), Default, "Invalid Index");

			return GetItem(0);
		}

		const T& First() const
		{
			NX_ASSERT(IsValidIndex(0), Default, "Invalid Index");

			return GetItem(0);
		}

		T& Last() 
		{
			NX_ASSERT(IsValidIndex(Count - 1), Default, "Invalid Index");

			return GetItem(Count - 1);
		}

		const T& Last() const
		{
			NX_ASSERT(IsValidIndex(Count - 1), Default, "Invalid Index");

			return GetItem(Count - 1);
		}

		I GetIterator(uint64 Index)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetIt(Index);
		}

		const I GetIterator(uint64 Index) const
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetIt(Index);
		}

		I begin() { return Begin(); }
		I Begin() 
		{
			return GetIt(0);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIt(0);
		}

		I BeginReverse()
		{
			I It = End();
			--It;
			return It;
		}

		const I BeginReverse() const
		{
			I It = End();
			--It;
			return It;
		}

		I end() { return End(); }
		I End() 
		{
			return GetIt(Count);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIt(Count);
		}

		I EndReverse()
		{
			I It = Begin();
			--It;
			return It;
		}

		const I EndReverse() const
		{
			I It = Begin();
			--It;
			return It;
		}

		bool IsValidIndex(uint64 Index) const
		{
			return !IsEmpty() && Index >= 0 && Index < Count;
		}

		void Reserve(uint64 Size, bool Shrink = false)
		{
			Size = Math::Max(Size, Count);
			if (Capacity >= Size && !Shrink)
			{
				return;
			}

			Reallocate(Size);
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Allocate(sizeof(T) * Capacity, Allctr);
		}

		void Reallocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Reallocate(Data, sizeof(T) * Capacity, Allctr);
		}

		void Free()
		{
			Memory::Free(Data, Allctr);
			Data = nullptr;
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
			if (!Forward && (Index + Size) >= Count)
			{
				return;
			}

			T* Start = Forward ? &Data[Index] : &Data[Index + Size];
			T* End = Forward ? &Data[Index + Size] : &Data[Index];
			Memory::MemMove(Start, End, sizeof(T) * (Count - Index - Size));
		}

		T& GetItem(uint64 Index)
		{
			return Data[Index];
		}

		const T& GetItem(uint64 Index) const
		{
			return Data[Index];
		}

		I GetIt(uint64 Index)
		{
			return I(Data, Index);
		}

		const I GetIt(uint64 Index) const
		{
			return I(Data, Index);
		}

		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				Reallocate(GrowPolicy());
			}
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

		Allocator* Allctr;
		uint64 Capacity;
		uint64 Count;
		T* Data;
	};
}
