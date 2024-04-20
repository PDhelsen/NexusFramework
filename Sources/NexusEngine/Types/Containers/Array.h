#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/Sort.h"
#include "Misc/References.h"

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

		Array(uint64 Cnt, Allocator* Alloc = nullptr)
			: Allocator(nullptr), Count(Cnt), Data(nullptr)
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

		Array<T> Copy() const
		{
			Array<T> Copy = Array<T>(Count, Allocator);
			for (uint64 Index = 0; Index < Count; Index++)
			{
				Copy.Data[Index] = Data[Index];
			}
			return Copy;
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

		template<typename... Args>
		void Initialize(Args&&... args)
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				T& Instance = Data[Index];
				Memory::Construct<T>(&Instance, args...);
			}
		}

		void Assign(uint64 Index, const T& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			Data[Index] = Value;
		}

		void Assign(uint64 Index, T&& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			Data[Index] = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(uint64 Index, Args&&... args)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			Data[Index] = T(args...);
		}

		void AssignRange(uint64 Index, Array<T>& Values)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(Index + Values.GetCount()  - 1), "Invalid Index");

			for (uint64 Offset = 0; Offset < Values.GetCount(); Offset++)
			{
				Data[Index + Offset] = Values[Offset];
			}
		}

		T& Get(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			return Data[Index];
		}

		T& First()
		{
			return Data[0];
		}

		T& Last()
		{
			return Data[Count - 1];
		}

		void Swap(uint64 IndexA, uint64 IndexB)
		{
			NEXUS_ASSERT(IsValidIndex(IndexA), "Invalid Index");
			NEXUS_ASSERT(IsValidIndex(IndexB), "Invalid Index");
			
			T Temp = Data[IndexB];
			Data[IndexB] = Move(Data[IndexA]);
			Data[IndexA] = Move(Temp);
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
			return Find(Other) < Count;
		}

		uint64 Find(const T& Other) const
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				if (Data[Index] == Other)
				{
					return Index;
				}
			}

			return Count;
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
		T* GetRaw() { return Data; }

	private:
		Allocator* Allocator;
		uint64 Count;
		T* Data;
	};
}