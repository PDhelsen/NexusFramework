#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T, uint64 BS = 10>
	class Dequeue
	{
	public:
		using Iterator = IteratorBucket<T, BS>;

		Dequeue(Allocator* Allctr = nullptr)
			: Alloc(nullptr), Buckets(0), Count(0), IndexFront(0), IndexBack(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			ValidateDefaultState();
			Allocate(1);
			Allocate(0, BucketSize);
		}

		Dequeue(const Dequeue<T>& Other)
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Dequeue - Copy constructor");

			Allocate(Buckets);

			for (uint64 Bucket = 0; Bucket < Buckets; Bucket++)
			{
				Allocate(Bucket, BucketSize);
			}

			for (uint64 Index = 0; Index < Count; Index++)
			{
				Construct(Index, Other[Index]);
			}
		}

		Dequeue(Dequeue<T>&& Other) noexcept
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Dequeue()
		{
			Clear();
			Free();
		}

		Dequeue<T>& operator=(const Dequeue<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Dequeue - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Alloc = Other.Alloc;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexFront = Other.IndexFront;
			IndexBack = Other.IndexBack;

			Allocate(Buckets);

			for (uint64 Bucket = 0; Bucket < Buckets; Bucket++)
			{
				Allocate(Bucket, BucketSize);
			}

			for (uint64 Index = 0; Index < Count; Index++)
			{
				Construct(Index, Other[Index]);
			}

			return *this;
		}

		Dequeue<T>& operator=(Dequeue<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Alloc = Other.Alloc;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexFront = Other.IndexFront;
			IndexBack = Other.IndexBack;
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

		bool operator==(const Dequeue<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dequeue<T>& Other) const
		{
			return !(*this == Other);
		}

		T& Assign(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = Value;
			return Data[BucketIndex][DataIndex];
		}

		T& Assign(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = Move(Value);
			return Data[BucketIndex][DataIndex];
		}

		template<typename... Args>
		T& AssignConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Destruct(Index, 1);
			Construct(Index, args...);
			return Data[BucketIndex][DataIndex];
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Value.GetCount() - 1), "Overflow");

			uint64 Offset = 0;
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++, Offset++)
			{
				uint64 BucketIndex, DataIndex;
				GetIndex(Index + Offset, BucketIndex, DataIndex);
				Data[BucketIndex][DataIndex] = *It;
			}
		
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		T& AppendBack(const T& Value)
		{
			AppendBucket(true);
			Construct(Count - 1, Value);
			return Data[Buckets - 1][IndexBack];
		}

		T& AppendBack(T&& Value)
		{
			AppendBucket(true);
			Construct(Count - 1, Move(Value));
			return Data[Buckets - 1][IndexBack];
		}

		template<typename... Args>
		T& AppendBackConstruct(Args&&... args)
		{
			AppendBucket(true);
			Construct(Count - 1, args...);
			return Data[Buckets - 1][IndexBack];
		}

		template<typename C>
		T& AppendBackRange(const C& Value)
		{
			uint64 BucketIndex = Buckets - 1;
			uint64 DataIndex = IndexBack;

			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				AppendBack(*It);
			}

			return Data[BucketIndex][DataIndex];
		}

		T& AppendFront(const T& Value)
		{
			AppendBucket(false);
			Construct(0, Value);
			return Data[0][IndexFront];
		}

		T& AppendFront(T&& Value)
		{
			AppendBucket(false);
			Construct(0, Move(Value));
			return Data[0][IndexFront];
		}

		template<typename... Args>
		T& AppendFrontConstruct(Args&&... args)
		{
			AppendBucket(false);
			Construct(0, args...);
			return Data[0][IndexFront];
		}

		template<typename C>
		T& AppendFrontRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				AppendFront(*It);
			}

			return Data[0][IndexFront];
		}

		void RemoveBack()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");

			Destruct(Count - 1, 1);
			RemoveBucket(true);
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			Destruct(0, 1);
			RemoveBucket(false);
		}

		void Clear()
		{
			Destruct(0, Count);
			Free(0, Buckets);
			
			ValidateDefaultState();
			Reallocate(1);
			Allocate(0, BucketSize);
		}

		T& Get(uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
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
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return Iterator(Data, IndexFront, BucketIndex, DataIndex);
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, IndexFront, 0, IndexFront);
		}

		Iterator BeginReverse() const
		{
			return --End();
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			Iterator It = Iterator(Data, IndexFront, Buckets - 1, IndexBack);
			return ++It;
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

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetBuckets() const { return Buckets; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Allocate(sizeof(T*) * Buckets, NEXUS_MEMORY_ALIGN, Alloc);
		}

		void Allocate(uint64 Index, uint64 Size)
		{
			Data[Index] = (T*)Memory::Allocate(sizeof(T) * Size, NEXUS_MEMORY_ALIGN, Alloc);
		}

		void Reallocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Realloc(Data, sizeof(T*) * Buckets, NEXUS_MEMORY_ALIGN, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
		}

		void Free(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; Offset++)
			{
				Memory::Free(Data[Index + Offset], Alloc);
			}
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Memory::Construct<T>(&Data[BucketIndex][DataIndex], args...);
		}

		void Destruct(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; Offset++)
			{
				uint64 BucketIndex, DataIndex;
				GetIndex(Index + Offset, BucketIndex, DataIndex);
				Memory::Destruct(&Data[BucketIndex][DataIndex]);
			}
		}

		void Shift(bool Forward)
		{
			T** Start = Forward ? &Data[0] : &Data[1];
			T** End = Forward ? &Data[1] : &Data[0];
			Memory::MemCopy(Start, End, sizeof(T*) * (Buckets - 1));
		}

		void AppendBucket(bool AppendBack)
		{
			Count++;

			if ((AppendBack && IndexBack == BucketSize - 1) || (!AppendBack && IndexFront == 0))
			{
				Reallocate(++Buckets);
				if (!AppendBack)
				{
					Shift(true);
				}
				Allocate(AppendBack ? Buckets - 1 : 0, BucketSize);

				if (AppendBack)
				{
					IndexBack = 0;
				}
				else
				{
					IndexFront = BucketSize - 1;
				}
			}
			else
			{
				if (AppendBack)
				{
					IndexBack++;
				}
				else
				{
					IndexFront--;
				}
			}
		}

		void RemoveBucket(bool RemoveBack)
		{
			Count--;

			if ((RemoveBack && IndexBack == 0) || (!RemoveBack && IndexFront == BucketSize - 1))
			{
				Free(RemoveBack ? Buckets - 1 : 0, 1);
				if (!RemoveBack)
				{
					Shift(false);
				}
				;
				Reallocate(--Buckets);

				if (RemoveBack)
				{
					IndexBack = BucketSize - 1;
				}
				else
				{
					IndexFront = 0;
				}
			}
			else
			{
				if (RemoveBack)
				{
					IndexBack--;
				}
				else
				{
					IndexFront++;
				}
			}
		}

		void GetIndex(uint64 Index, uint64& BucketIndex, uint64& DataIndex) const
		{
			Index += IndexFront;
			BucketIndex = Index / BucketSize;
			DataIndex = Index % BucketSize;
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		void ValidateBucket(uint64 Size)
		{
			Buckets = Size > 1 ? Size : 1;
		}

		void ValidateDefaultState()
		{
			Count = 0;
			IndexFront = 5;
			IndexBack = 4;
		}

		inline static const uint64 BucketSize = BS;

		Allocator* Alloc;
		uint64 Buckets;
		uint64 Count;
		uint64 IndexFront;
		uint64 IndexBack;
		T** Data;
	};
}
