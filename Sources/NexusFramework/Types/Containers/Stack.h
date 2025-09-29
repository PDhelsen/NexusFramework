#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Types/Containers/Iterator.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename T, uint64 BS = 10>
	class Stack
	{
	public:
		using I = Iterator::IteratorBucket<T, BS>;

		inline static const uint64 BucketSize = BS;

		Stack(Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Buckets(0), Count(0), IndexLast(0), Data(nullptr)
		{
			ValidateDefaultState();
		}

		Stack(InitializerList<T> Init, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Buckets(0), Count(0), IndexLast(0), Data(nullptr)
		{
			ValidateDefaultState();

			for (auto& It : Init)
			{
				Append(It);
			}
		}

		Stack(const Stack<T, BS>& Other)
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexLast(Other.IndexLast), Data(nullptr)
		{
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
				Construct(BucketIndex, DataIndex, Other.GetItem(Index));
			}
		}

		Stack(Stack<T, BS>&& Other) noexcept
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexLast(Other.IndexLast), Data(Other.Data)
		{
			Other.ValidateDefaultState();
			Other.Data = nullptr;
		}

		~Stack()
		{
			Clear();
		}

		Stack<T, BS>& operator=(const Stack<T, BS>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

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
				Construct(BucketIndex, DataIndex, Other.GetItem(Index));
			}

			return *this;
		}

		Stack<T, BS>& operator=(Stack<T, BS>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexLast = Other.IndexLast;
			Data = Other.Data;

			Other.ValidateDefaultState();
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

		bool operator>(const Stack<T, BS>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Stack<T, BS>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Stack<T, BS>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Stack<T, BS>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Append(const T& Value)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexLast, Value);
			return GetItem(Buckets - 1, IndexLast);
		}

		T& Append(T&& Value)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexLast, Move(Value));
			return GetItem(Buckets - 1, IndexLast);
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexLast, args...);
			return GetItem(Buckets - 1, IndexLast);
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			uint64 Index = Count;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				Append(*It);
			}

			return GetItem(Buckets - 1, IndexLast);
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Stack is Empty");

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

		T& Get()
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Stack is Empty");

			return GetItem(Buckets - 1, IndexLast);
		}

		const T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Stack is Empty");

			return GetItem(Buckets - 1, IndexLast);
		}

		I begin() { return Begin(); }
		I Begin()
		{
			return GetIteratorIndex(0, 0);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIteratorIndex(0, 0);
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
			I It = GetIteratorIndex(Buckets - 1, IndexLast);
			return ++It;
		}

		const I end() const { return End(); }
		const I End() const
		{
			I It = GetIteratorIndex(Buckets - 1, IndexLast);
			return ++It;
		}

		I EndReverse()
		{
			return --Begin();
		}

		const I EndReverse() const
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

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetBuckets() const { return Buckets; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Allocate(sizeof(T*) * Buckets, Alloc);
		}

		void Allocate(uint64 Index, uint64 Size)
		{
			Data[Index] = (T*)Memory::Allocate(sizeof(T) * Size, Alloc);
		}

		void Reallocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Reallocate(Data, sizeof(T*) * Buckets, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
			Data = nullptr;
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

		T& GetItem(uint64 Index) const
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		T& GetItem(uint64 BucketIndex, uint64 DataIndex) const
		{
			return Data[BucketIndex][DataIndex];
		}

		I GetIteratorIndex(uint64 Index) const
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return I(Data, 0, BucketIndex, DataIndex);
		}

		I GetIteratorIndex(uint64 BucketIndex, uint64 DataIndex) const
		{
			return I(Data, 0, BucketIndex, DataIndex);
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

		void ValidateBucket(uint64 Size)
		{
			Buckets = Size > 1 ? Size : 1;
		}

		void ValidateDefaultState()
		{
			Count = 0;
			Buckets = 0;
			IndexLast = BucketSize - 1;
		}

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Count;
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			T Temp = GetItem(IndexA);
			GetItem(IndexA) = Move(GetItem(IndexB));
			GetItem(IndexB) = Move(Temp);
		}

		Allocator* Alloc;
		uint64 Buckets;
		uint64 Count;
		uint64 IndexLast;
		T** Data;
	};
}
