#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Dequeue
	{
	public:
		class Iterator
		{
		public:
			Iterator(T** Ptr, uint64 BucketIdx, uint64 DataIdx)
				: Pointer(Ptr), BucketIndex(BucketIdx), DataIndex(DataIdx)
			{

			}

			Iterator& operator++()
			{
				if (DataIndex == BucketSize - 1)
				{
					BucketIndex++;
					DataIndex = 0;
				}
				else
				{
					DataIndex++;
				}
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
				if (DataIndex == 0)
				{
					BucketIndex--;
					DataIndex = BucketSize - 1;
				}
				else
				{
					DataIndex--;
				}
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
				return &Pointer[BucketIndex][DataIndex];
			}

			T& operator*() const
			{
				return Pointer[BucketIndex][DataIndex];
			}

			bool operator==(const Iterator& Other) const
			{
				return Pointer == Other.Pointer && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
			}

			bool operator!=(const Iterator& Other) const
			{
				return Pointer != Other.Pointer || BucketIndex != Other.BucketIndex || DataIndex != Other.DataIndex;
			}

		private:
			T** Pointer;
			uint64 BucketIndex;
			uint64 DataIndex;
		};

		Dequeue(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Buckets(0), Count(0), IndexFront(0), IndexBack(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			ValidateDefaultState();
			Allocate(1);
			Allocate(0, BucketSize);
		}

		Dequeue(const Dequeue<T>& Other)
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(Other.Data)
		{
		}

		Dequeue(Dequeue<T>&& Other) noexcept
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Dequeue()
		{
			Free();
		}

		Dequeue<T> Copy() const
		{
			Dequeue<T> Copy = Dequeue(Allocator);
			
			Copy.Buckets = Buckets;
			Copy.Count = Count;
			Copy.IndexFront = IndexFront;
			Copy.IndexBack = IndexBack;

			Copy.Reallocate(Buckets);
			for (uint64 Bucket = 1; Bucket < Buckets; Bucket++)
			{
				Copy.Allocate(Bucket, BucketSize);
			}

			for (uint64 Bucket = 0; Bucket < Buckets; Bucket++)
			{
				for (uint64 Index = 0; Index < BucketSize; Index++)
				{
					Copy.Data[Bucket][Index] = Data[Bucket][Index];
				}
			}

			return Copy;
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

		void Assign(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = Value;
		}

		void Assign(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = T(args...);
		}

		void AssignRange(uint64 Index, const Dequeue<T>& Values)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Values.Count), "Overflow");

			for (uint64 Offset = 0; Offset < Values.Count; Offset++)
			{
				Assign(Index + Offset, Values[Offset]);
			}
		}

		void AppendBack(const T& Value)
		{
			AppendBucket(true);
			Data[Buckets - 1][IndexBack] = Value;
		}

		void AppendBack(T&& Value)
		{
			AppendBucket(true);
			Data[Buckets - 1][IndexBack] = Move(Value);
		}

		template<typename... Args>
		void AppendBackConstruct(Args&&... args)
		{
			AppendBucket(true);
			Data[Buckets - 1][IndexBack] = T(args...);
		}

		void AppendBackRange(const Dequeue<T>& Value)
		{
			for (uint64 Offset = 0; Offset < Value.Count; Offset++)
			{
				AppendBack(Value[Offset]);
			}
		}

		void AppendFront(const T& Value)
		{
			AppendBucket(false);
			Data[0][IndexFront] = Value;
		}

		void AppendFront(T&& Value)
		{
			AppendBucket(false);
			Data[0][IndexFront] = Move(Value);
		}

		template<typename... Args>
		void AppendFrontConstruct(Args&&... args)
		{
			AppendBucket(false);
			Data[0][IndexFront] = T(args...);
		}

		void AppendFrontRange(const Dequeue<T>& Value)
		{
			for (uint64 Offset = 0; Offset < Value.Count; Offset++)
			{
				AppendFront(Value[Value.Count - 1 - Offset]);
			}
		}

		void RemoveBack()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");

			RemoveBucket(true);
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			RemoveBucket(false);
		}

		void Clear()
		{
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Free(Index);
			}
			
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

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, 0, IndexFront);
		}

		Iterator BeginReverse() const
		{
			return --End();
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			Iterator It = Iterator(Data, Buckets - 1, IndexBack);
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

		bool Contains(const T& Other) const
		{
			return Find(Other) < Count;
		}

		uint64 Find(const T& Other) const
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				uint64 BucketIndex, DataIndex;
				GetIndex(Index, BucketIndex, DataIndex);
				if (Data[BucketIndex][DataIndex] == Other)
				{
					return Index;
				}
			}

			return Count;
		}

		uint64 GetCount() const { return Count; }
		uint64 GetBuckets() const { return Buckets; }
		bool IsEmpty() const { return Count == 0; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Allocate(sizeof(T*) * Buckets, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Allocate(uint64 Index, uint64 Size)
		{
			Data[Index] = (T*)Memory::Allocate(sizeof(T) * Size, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Reallocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Realloc(Data, sizeof(T*) * Buckets, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Free()
		{
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Free(Index);
			}

			Memory::Free(Data, Allocator);
		}

		void Free(uint64 Index)
		{
			Memory::Free(Data[Index], Allocator);
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
				Free(RemoveBack ? Buckets - 1 : 0);
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
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
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

		static const uint64 BucketSize = 10;

		Allocator* Allocator;
		uint64 Buckets;
		uint64 Count;
		uint64 IndexFront;
		uint64 IndexBack;
		T** Data;
	};
}