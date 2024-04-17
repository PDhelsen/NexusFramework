#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
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
			: Allocator(nullptr), Count(Count), Data(nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
			Data = (T*)Memory::Allocate(sizeof(T) * Count, NEXUS_MEMORY_ALIGN, Allocator);
		}

		Array(const Array<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
		}

		Array(Array<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
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

		Array<T> CopyShallow() const
		{
			return Array(*this);
		}

		Array<T> CopyDeep() const
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
			Data[IndexB] = (T&&)Data[IndexA];
			Data[IndexA] = (T&&)Temp;
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

		bool IsValidIndex(uint64 Index) const
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

		void Reverse()
		{
			uint64 Half = Count / 2;
			for (uint64 Front = 0, Back = Count - 1; Front < Half; Front++, Back--)
			{
				Swap(Front, Back);
			}
		}

		uint64 GetCount() const { return Count; }

	private:
		Allocator* Allocator;
		uint64 Count;
		T* Data;
	};
}