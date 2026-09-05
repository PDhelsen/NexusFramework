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
	template<typename T, uint64 BS = 10>
	class Queue
	{
	public:
		using I = Iterator::Bucket<T, BS>;

		inline static const uint64 BucketSize = BS;

		Queue(Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Buckets(0), Count(0), IndexFront(0), IndexBack(0), Data(nullptr)
		{
			ValidateDefaultState();
		}

		Queue(InitializerList<T> Init, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Buckets(0), Count(0), IndexFront(0), IndexBack(0), Data(nullptr)
		{
			ValidateDefaultState();

			for (auto& It : Init)
			{
				AppendConstruct(It);
			}
		}

		Queue(const Queue<T, BS>& Other)
			: Allctr(Other.Allctr), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(nullptr)
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

		Queue(Queue<T, BS>&& Other) noexcept
			: Allctr(Other.Allctr), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(Other.Data)
		{
			Other.ValidateDefaultState();
			Other.Data = nullptr;
		}

		~Queue()
		{
			Clear();
		}

		Queue<T, BS>& operator=(const Queue<T, BS>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexFront = Other.IndexFront;
			IndexBack = Other.IndexBack;

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

		Queue<T, BS>& operator=(Queue<T, BS>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

			Allctr = Other.Allctr;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexFront = Other.IndexFront;
			IndexBack = Other.IndexBack;
			Data = Other.Data;

			Other.ValidateDefaultState();
			Other.Data = nullptr;

			return *this;
		}

		bool operator==(const Queue<T, BS>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Queue<T, BS>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Queue<T, BS>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Queue<T, BS>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Queue<T, BS>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Queue<T, BS>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Append(const T& Value)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexBack, Value);
			return GetItem(Buckets - 1, IndexBack);
		}

		T& Append(T&& Value)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexBack, Move(Value));
			return GetItem(Buckets - 1, IndexBack);
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			AppendBucket();
			Construct(Buckets - 1, IndexBack, args...);
			return GetItem(Buckets - 1, IndexBack);
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			uint64 Index = Count;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				Append(*It);
			}

			return GetItem(Buckets - 1, IndexBack);
		}

		void Remove()
		{
			NX_ASSERT(!IsEmpty(), Default, "Queue is Empty");

			Destruct(0, IndexFront);
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
			NX_ASSERT(!IsEmpty(), Default, "Queue is Empty");

			return GetItem(0, IndexFront);
		}

		const T& Get() const
		{
			NX_ASSERT(!IsEmpty(), Default, "Queue is Empty");

			return GetItem(0, IndexFront);
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

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetBuckets() const { return Buckets; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Allocate(sizeof(T*) * Buckets, Allctr);
		}

		void Allocate(uint64 Index, uint64 Size)
		{
			Data[Index] = (T*)Memory::Allocate(sizeof(T) * Size, Allctr);
		}

		void Reallocate(uint64 Size)
		{
			ValidateBucket(Size);
			Data = (T**)Memory::Reallocate(Data, sizeof(T*) * Buckets, Allctr);
		}

		void Free()
		{
			Memory::Free(Data, Allctr);
			Data = nullptr;
		}

		void Free(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Free(Data[Index + Offset], Allctr);
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

		void Shift()
		{
			T** Start = &Data[1];
			T** End = &Data[0];
			Memory::MemMove(Start, End, sizeof(T*) * (Buckets - 1));
		}

		void AppendBucket()
		{
			++Count;

			if (IndexBack == BucketSize - 1)
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

				IndexBack = 0;
			}
			else
			{
				++IndexBack;

			}
		}

		void RemoveBucket()
		{
			--Count;

			if (IndexFront == BucketSize - 1)
			{
				Free(0, 1);
				if (Buckets > 1)
				{
					Shift();
				}
				--Buckets;
				if (Buckets > 0)
				{
					Reallocate(Buckets);
				}
				else
				{
					Free();
				}

				IndexFront = 0;
			}
			else
			{
				++IndexFront;
			}
		}

		void GetIndex(uint64 Index, uint64& BucketIndex, uint64& DataIndex) const
		{
			Index += IndexFront;
			BucketIndex = Index / BucketSize;
			DataIndex = Index % BucketSize;
		}

		T& GetItem(uint64 Index)
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		const T& GetItem(uint64 Index) const
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		T& GetItem(uint64 BucketIndex, uint64 DataIndex)
		{
			return Data[BucketIndex][DataIndex];
		}

		const T& GetItem(uint64 BucketIndex, uint64 DataIndex) const
		{
			return Data[BucketIndex][DataIndex];
		}

		I GetIt(uint64 Index)
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return I(Data, IndexFront, BucketIndex, DataIndex);
		}

		const I GetIt(uint64 Index) const
		{
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			return I(Data, IndexFront, BucketIndex, DataIndex);
		}

		void ValidateBucket(uint64 Size)
		{
			Buckets = Size > 1 ? Size : 1;
		}

		void ValidateDefaultState()
		{
			Count = 0;
			Buckets = 0;
			IndexFront = 0;
			IndexBack = BucketSize - 1;
		}

		Allocator* Allctr;
		uint64 Buckets;
		uint64 Count;
		uint64 IndexFront;
		uint64 IndexBack;
		T** Data;
	};
}
