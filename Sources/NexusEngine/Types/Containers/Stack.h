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
	class Stack
	{
	public:
		using I = Iterator::IteratorBucket<T, BS>;

		Stack(Allocator* Allctr = nullptr)
			: Alloc(nullptr), Buckets(0), Count(0), IndexLast(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			ValidateDefaultState();
		}

		Stack(const Stack<T, BS>& Other)
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexLast(Other.IndexLast), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Stack - Copy constructor");

			if (Buckets)
			{
				Allocate(Buckets);
			}

			for (uint64 Bucket = 0; Bucket < Buckets; ++Bucket)
			{
				Allocate(Bucket, BucketSize);
			}

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				uint64 BucketIndex, DataIndex;
				GetIndex(Index, BucketIndex, DataIndex);
				Construct(BucketIndex, DataIndex, Other.Data[BucketIndex][DataIndex]);
			}
		}

		Stack(Stack<T, BS>&& Other) noexcept
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexLast(Other.IndexLast), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Stack()
		{
			Clear();
		}

		Stack<T, BS>& operator=(const Stack<T, BS>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Stack - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexLast = Other.IndexLast;

			if (Buckets)
			{
				Allocate(Buckets);
			}

			for (uint64 Bucket = 0; Bucket < Buckets; ++Bucket)
			{
				Allocate(Bucket, BucketSize);
			}

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				uint64 BucketIndex, DataIndex;
				GetIndex(Index, BucketIndex, DataIndex);
				Construct(BucketIndex, DataIndex, Other.Data[BucketIndex][BucketIndex]);
			}

			return *this;
		}

		Stack<T, BS>& operator=(Stack<T, BS>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexLast = Other.IndexLast;
			Data = Other.Data;

			Other.Capacity = 0;
			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
		}

		bool operator==(const Stack<T, BS>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Stack<T, BS>& Other) const
		{
			return !(*this == Other);
		}

		T& Append(const T& Value)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexLast, Value);
			return Data[Buckets - 1][IndexLast];
		}

		T& Append(T&& Value)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexLast, Move(Value));
			return Data[Buckets - 1][IndexLast];
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexLast, args...);
			return Data[Buckets - 1][IndexLast];
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			uint64 Index = Count;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				Append(*It);
			}

			return Data[Buckets - 1][IndexLast];
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), "Stack is Empty");

			Destruct(Buckets - 1, IndexLast);
			RemoveBucket();
		}

		void Clear()
		{
			DestructRange(0, Count);
			Free(0, Buckets);
			Free();

			ValidateDefaultState();
		}

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Stack is Empty");

			return Data[Buckets - 1][IndexLast];
		}

		I begin() const { return Begin(); }
		I Begin() const
		{
			return I(Data, 0, 0, 0);
		}

		I BeginReverse() const
		{
			return --End();
		}

		I end() const { return End(); }
		I End() const
		{
			I It = I(Data, 0, Buckets - 1, IndexLast);
			return ++It;
		}

		I EndReverse() const
		{
			return --Begin();
		}

		void Reverse()
		{
			uint64 Half = Count / 2;
			for (uint64 Front = 0, Back = Count - 1; Front < Half; ++Front, --Back)
			{
				Swap(Front, Back);
			}
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
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Free(Data[Index + Offset], Alloc);
			}
		}

		template<typename... Args>
		void Construct(uint64 BucketIndex, uint64 DataIndex, Args&&... args)
		{
			Memory::Construct<T>(&Data[BucketIndex][DataIndex], args...);
		}

		void Destruct(uint64 BucketIndex, uint64 DataIndex)
		{
			Memory::Destruct(&Data[BucketIndex][DataIndex]);
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				uint64 BucketIndex, DataIndex;
				GetIndex(Index + Offset, BucketIndex, DataIndex);
				Memory::Destruct(&Data[BucketIndex][DataIndex]);
			}
		}

		void AppendBucket()
		{
			++Count;

			if (IndexLast == BucketSize - 1)
			{
				++Buckets;
				if (Buckets <= 1)
				{
					Allocate(Buckets);
				}
				else
				{
					Reallocate(Buckets);
				}
				Allocate(Buckets - 1, BucketSize);

				IndexLast = 0;
			}
			else
			{
				++IndexLast;

			}
		}

		void RemoveBucket()
		{
			--Count;

			if (IndexLast == 0)
			{
				Free(Buckets - 1, 1);
				--Buckets;
				if (Buckets > 0)
				{
					Reallocate(Buckets);
				}
				else
				{
					Free();
				}

				IndexLast = BucketSize - 1;
			}
			else
			{
				--IndexLast;
			}
		}

		void GetIndex(uint64 Index, uint64& BucketIndex, uint64& DataIndex) const
		{
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
			IndexLast = BucketSize - 1;
		}

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Count;
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			NEXUS_ASSERT(IsValidIndex(IndexA), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(IndexB), "Invalid Index");

			uint64 BucketIndexA, DataIndexA;
			GetIndex(IndexA, BucketIndexA, DataIndexA);
			uint64 BucketIndexB, DataIndexB;
			GetIndex(IndexB, BucketIndexB, DataIndexB);

			T Temp = Data[BucketIndexA][DataIndexA];
			Data[BucketIndexA][DataIndexA] = Move(Data[BucketIndexB][DataIndexB]);
			Data[BucketIndexB][DataIndexB] = Move(Temp);
		}

		inline static const uint64 BucketSize = BS;

		Allocator* Alloc;
		uint64 Buckets;
		uint64 Count;
		uint64 IndexLast;
		T** Data;
	};
}
