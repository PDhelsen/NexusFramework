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

			T* operator->()
			{
				return &Pointer[BucketIndex][DataIndex];
			}

			T& operator*()
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
			: Allocator(nullptr), Buckets(0), Count(0), Front(0), Back(0), Data(nullptr)
		{
			Reset();
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
			Data = (T**)Memory::Allocate(sizeof(T*) * Buckets, NEXUS_MEMORY_ALIGN, Allocator);
			Data[0] = (T*)Memory::Allocate(sizeof(T) * BucketSize, NEXUS_MEMORY_ALIGN, Allocator);
		}

		Dequeue(const Dequeue<T>& Other)
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), Front(Other.Front), Back(Other.Back), Data(Other.Data)
		{
		}

		Dequeue(Dequeue<T>&& Other) noexcept
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), Front(Other.Front), Back(Other.Back), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Dequeue()
		{
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Memory::Free(Data[Index], Allocator);
			}

			Memory::Free(Data, Allocator);
		}

		Dequeue<T> Copy() const
		{
			Dequeue<T> Copy = Dequeue(Allocator);
			
			Copy.Buckets = Buckets;
			Copy.Count = Count;
			Copy.Front = Front;
			Copy.Back = Back;

			Copy.ResizeBuckets();
			for (uint64 Bucket = 1; Bucket < Buckets; Bucket++)
			{
				Copy.CreateBucket(Bucket);
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

		T& operator[](uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		const T& operator[](uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		bool operator==(const Dequeue<T>& Other)
		{
			return Buckets == Other.Buckets && Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dequeue<T>& Other)
		{
			return Buckets != Other.Buckets || Count != Other.Count || Data != Other.Data;
		}

		void Assign(uint64 Index, const T& Value)
		{
			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = Value;
		}

		void Assign(uint64 Index, T&& Value)
		{
			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(uint64 Index, Args&&... args)
		{
			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			Data[BucketIndex][DataIndex] = T(args...);
		}

		void AssignRange(uint64 Index, const Dequeue<T>& Values)
		{
			NEXUS_ASSERT(IsValidIndex(Index + Values.Count), "Overflow");

			for (uint64 Offset = 0; Offset < Values.Count; Offset++)
			{
				Assign(Index + Offset, Values[Offset]);
			}
		}

		void AppendBack(const T& Value)
		{
			Grow(true);
			Data[Buckets - 1][Back] = Value;
		}

		void AppendBack(T&& Value)
		{
			Grow(true);
			Data[Buckets - 1][Back] = Move(Value);
		}

		template<typename... Args>
		void AppendBackConstruct(Args&&... args)
		{
			Grow(true);
			Data[Buckets - 1][Back] = T(args...);
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
			Grow(false);
			Data[0][Front] = Value;
		}

		void AppendFront(T&& Value)
		{
			Grow(false);
			Data[0][Front] = Move(Value);
		}

		template<typename... Args>
		void AppendFrontConstruct(Args&&... args)
		{
			Grow(false);
			Data[0][Front] = T(args...);
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

			Shrink(true);
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			Shrink(false);
		}

		void Clear()
		{
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Memory::Free(Data[Index], Allocator);
			}
			
			Reset();
			ResizeBuckets();
			CreateBucket(0);
		}

		T& Get(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		const T& Get(uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 BucketIndex, DataIndex;
			ConvertIndex(Index, BucketIndex, DataIndex);
			return Data[BucketIndex][DataIndex];
		}

		T& First()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			return Data[0][Front];
		}

		T& Last()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			return Data[Buckets - 1][Back];
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, 0, Front);
		}

		Iterator BeginReverse() const
		{
			return --End();
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			Iterator It = Iterator(Data, Buckets - 1, Back);
			return ++It;
		}

		Iterator EndReverse() const
		{
			return --Begin();
		}

		bool IsValidIndex(uint64 Index)
		{
			return Index >= 0 && Index < Count;
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
				ConvertIndex(Index, BucketIndex, DataIndex);
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
		void CreateBucket(uint64 Index)
		{
			Data[Index] = (T*)Memory::Allocate(sizeof(T) * BucketSize, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void DestroyBucket(uint64 Index)
		{
			Memory::Free(Data[Index], Allocator);
		}

		void ResizeBuckets()
		{
			Data = (T**)Memory::Realloc(Data, sizeof(T*) * Buckets, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void MoveBuckets(bool Forward)
		{
			T** Start = Forward ? &Data[0] : &Data[1];
			T** End = Forward ? &Data[1] : &Data[0];
			Memory::MemCopy(Start, End, sizeof(T*) * (Buckets - 1));
		}

		void ConvertIndex(uint64 Index, uint64& BucketIndex, uint64& DataIndex) const
		{
			Index += Front;
			BucketIndex = Index / BucketSize;
			DataIndex = Index % BucketSize;
		}

		void Grow(bool AppendBack)
		{
			Count++;

			if ((AppendBack && Back == BucketSize - 1) || (!AppendBack && Front == 0))
			{
				Buckets++;
				ResizeBuckets();
				if (!AppendBack)
				{
					MoveBuckets(true);
				}
				CreateBucket(AppendBack ? Buckets - 1 : 0);

				if (AppendBack)
				{
					Back = 0;
				}
				else
				{
					Front = BucketSize - 1;
				}
			}
			else
			{
				if (AppendBack)
				{
					Back++;
				}
				else
				{
					Front--;
				}
			}
		}

		void Shrink(bool RemoveBack)
		{
			Count--;

			if ((RemoveBack && Back == 0) || (!RemoveBack && Front == BucketSize - 1))
			{
				DestroyBucket(RemoveBack ? Buckets - 1 : 0);
				if (!RemoveBack)
				{
					MoveBuckets(false);
				}
				Buckets--;
				ResizeBuckets();

				if (RemoveBack)
				{
					Back = BucketSize - 1;
				}
				else
				{
					Front = 0;
				}
			}
			else
			{
				if (RemoveBack)
				{
					Back--;
				}
				else
				{
					Front++;
				}
			}
		}

		void Reset()
		{
			Buckets = 1;
			Count = 0;
			Front = 5;
			Back = 4;
		}

		static const uint64 BucketSize = 10;

		Allocator* Allocator;
		uint64 Buckets;
		uint64 Count;
		uint64 Front;
		uint64 Back;
		T** Data;
	};
}