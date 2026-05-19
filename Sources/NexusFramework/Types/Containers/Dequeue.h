#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Containers/Misc/Iterator.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename T, uint64 BS = 10>
	class Dequeue
	{
	public:
		using I = Iterator::IteratorBucket<T, BS>;

		inline static const uint64 BucketSize = BS;

		Dequeue(Allocator* Allctr = Allocator::TryGet())
			: Alloc(Allctr), Buckets(0), Count(0), IndexFront(0), IndexBack(0), Data(nullptr)
		{
			ValidateDefaultState();
		}

		Dequeue(InitializerList<T> Init, Allocator* Allctr = Allocator::TryGet())
			: Alloc(Allctr), Buckets(0), Count(0), IndexFront(0), IndexBack(0), Data(nullptr)
		{
			ValidateDefaultState();

			for (auto& It : Init)
			{
				AppendBack(It);
			}
		}

		Dequeue(const Dequeue<T, BS>& Other)
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(nullptr)
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

		Dequeue(Dequeue<T, BS>&& Other) noexcept
			: Alloc(Other.Alloc), Buckets(Other.Buckets), Count(Other.Count), IndexFront(Other.IndexFront), IndexBack(Other.IndexBack), Data(Other.Data)
		{
			Other.ValidateDefaultState();
			Other.Data = nullptr;
		}

		~Dequeue()
		{
			Clear();
		}

		Dequeue<T, BS>& operator=(const Dequeue<T, BS>& Other)
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

		Dequeue<T, BS>& operator=(Dequeue<T, BS>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;
			Buckets = Other.Buckets;
			Count = Other.Count;
			IndexFront = Other.IndexFront;
			IndexBack = Other.IndexBack;
			Data = Other.Data;

			Other.ValidateDefaultState();
			Other.Data = nullptr;

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

		bool operator==(const Dequeue<T, BS>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dequeue<T, BS>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Dequeue<T, BS>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Dequeue<T, BS>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Dequeue<T, BS>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Dequeue<T, BS>& Other) const
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
			
			uint64 BucketIndex, DataIndex;
			GetIndex(Index, BucketIndex, DataIndex);
			Destruct(BucketIndex, DataIndex);
			Construct(BucketIndex, DataIndex, args...);
			return GetItem(BucketIndex, DataIndex);
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");
			NX_ASSERT(IsValidIndex(Index + Value.GetCount() - 1), Default, "Invalid Index");

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				T& Item = GetItem(Index);
				Item = *It;
			}
		
			return GetItem(Index);
		}

		T& AppendBack(const T& Value)
		{
			AppendBucket(true);
			Construct(Buckets - 1, IndexBack, Value);
			return GetItem(Buckets - 1, IndexBack);
		}

		T& AppendBack(T&& Value)
		{
			AppendBucket(true);
			Construct(Buckets - 1, IndexBack, Move(Value));
			return GetItem(Buckets - 1, IndexBack);
		}

		template<typename... Args>
		T& AppendBackConstruct(Args&&... args)
		{
			AppendBucket(true);
			Construct(Buckets - 1, IndexBack, args...);
			return GetItem(Buckets - 1, IndexBack);
		}

		template<typename C>
		T& AppendBackRange(const C& Value)
		{
			uint64 Index = Count;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				AppendBack(*It);
			}

			return GetItem(Index);
		}

		T& AppendFront(const T& Value)
		{
			AppendBucket(false);
			Construct(0, IndexFront, Value);
			return GetItem(0, IndexFront);
		}

		T& AppendFront(T&& Value)
		{
			AppendBucket(false);
			Construct(0, IndexFront, Move(Value));
			return GetItem(0, IndexFront);
		}

		template<typename... Args>
		T& AppendFrontConstruct(Args&&... args)
		{
			AppendBucket(false);
			Construct(0, IndexFront, args...);
			return GetItem(0, IndexFront);
		}

		template<typename C>
		T& AppendFrontRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				AppendFront(*It);
			}

			return GetItem(Value.Count - 1);
		}

		void RemoveBack()
		{
			NX_ASSERT(!IsEmpty(), Default, "Dequeue is Empty");

			Destruct(Buckets - 1, IndexBack);
			RemoveBucket(true);
		}

		void RemoveFront()
		{
			NX_ASSERT(!IsEmpty(), Default, "Dequeue is Empty");
			
			Destruct(0, IndexFront);
			RemoveBucket(false);
		}

		void Clear()
		{
			DestructRange(0, Count);
			Free(0, Buckets);
			Free();

			ValidateDefaultState();
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

		void Shift(bool Forward)
		{
			T** Start = Forward ? &Data[0] : &Data[1];
			T** End = Forward ? &Data[1] : &Data[0];
			Memory::MemMove(Start, End, sizeof(T*) * (Buckets - 1));
		}

		void AppendBucket(bool AppendBack)
		{
			++Count;

			if ((AppendBack && IndexBack == BucketSize - 1) || (!AppendBack && IndexFront == 0))
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
				if (!AppendBack && Buckets > 1)
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
					++IndexBack;
				}
				else
				{
					--IndexFront;
				}
			}
		}

		void RemoveBucket(bool RemoveBack)
		{
			--Count;

			if ((RemoveBack && IndexBack == 0) || (!RemoveBack && IndexFront == BucketSize - 1))
			{
				Free(RemoveBack ? Buckets - 1 : 0, 1);
				if (!RemoveBack && Buckets > 1)
				{
					Shift(false);
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
					--IndexBack;
				}
				else
				{
					++IndexFront;
				}
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

		Allocator* Alloc;
		uint64 Buckets;
		uint64 Count;
		uint64 IndexFront;
		uint64 IndexBack;
		T** Data;
	};
}
