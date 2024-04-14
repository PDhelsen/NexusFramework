#pragma once

#include "Types/Integer.h"
#include "Debug/Assert.h"
#include "Misc/Sort.h"

namespace NxEn
{
	template<typename T>
	class Array
	{
	public:
		class Iterator
		{
		public:
			Iterator(T* Ptr)
				: Pointer(Ptr)
			{

			}

			Iterator& operator++()
			{
				Pointer++;
				return *this;
			}

			Iterator operator++(int32)
			{
				Iterator Copy = *this;
				++(*this);
				return Copy;
			}

			Iterator& operator--()
			{
				Pointer--;
				return *this;
			}

			Iterator operator--(int32)
			{
				Iterator Copy = *this;
				--(*this);
				return Copy;
			}

			T& operator[](uint64 Index)
			{
				return *(Pointer + Index);
			}

			T* operator->()
			{
				return Pointer;
			}

			T& operator*()
			{
				return *Pointer;
			}

			bool operator==(const Iterator& Other) const
			{
				return Pointer == Other.Pointer;
			}

			bool operator!=(const Iterator& Other) const
			{
				return Pointer != Other.Pointer;
			}

		private:
			T* Pointer;
		};

		Array(uint64 Count, Allocator* Alloc = nullptr)
			: Count(Count)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
			Data = (T*)Memory::Allocate(sizeof(T) * Count, NEXUS_MEMORY_ALIGN, Allocator);
		}

		Array(const Array<T>& Other)
			: Data(Other.Data), Count(Other.Count), Allocator(Other.Allocator)
		{
		}

		Array(Array<T>&& Other) noexcept
			: Data(Other.Data), Count(Other.Count), Allocator(Other.Allocator)
		{
			Other.Data = nullptr;
		}

		~Array()
		{
			Memory::Free(Data, Allocator);
		}

		template<typename... Args>
		void Initialize(Args&&... args)
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				T& Instance = Data[Index];
				Memory::Construct<T>(&Instance, args...);
			}
		}

		Array<T> CopyShallow()
		{
			return Array(*this);
		}

		Array<T> CopyDeep()
		{
			Array<T> New = Array(Count, Allocator);
			for (uint64 Index = 0; Index < Count; Index++)
			{
				New.Data[Index] = Data[Index];
			}
			return New;
		}

		T& operator[](uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			return Data[Index];
		}

		bool operator==(const Array<T>& Other)
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Array<T>& Other)
		{
			return Count != Other.Count || Data != Other.Data;
		}

		T& Get(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			return Data[Index];
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			NEXUS_ASSERT(IsValidIndex(IndexA), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(IndexB), "Invalid Index");
			
			T Temp = Data[IndexB];
			Data[IndexB] = Data[IndexA];
			Data[IndexA] = Temp;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data);
		}

		Iterator BeginReverse() const
		{
			return --End();
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data + Count);
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
			uint64 Index;
			return Find(Other, Index);
		}

		bool Find(const T& Other, uint64& Found) const
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				if (Data[Index] == Other)
				{
					Found = Index;
					return true;
				}
			}

			return false;
		}

		void Sort()
		{
			Sort::HeapSort(Data, Count);
		}

		uint64 GetCount() { return Count; }

	private:
		Allocator* Allocator;
		uint64 Count;
		T* Data;
	};
}