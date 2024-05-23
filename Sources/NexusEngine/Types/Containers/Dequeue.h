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
			Iterator(T** Ptr, uint64 ChunkIdx, uint64 DataIdx)
				: Pointer(Ptr), ChunkIndex(ChunkIdx), DataIndex(DataIdx)
			{

			}

			Iterator& operator++()
			{
				if (DataIndex == ChunkSize - 1)
				{
					ChunkIndex++;
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
					ChunkIndex--;
					DataIndex = ChunkSize - 1;
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
				return &Pointer[ChunkIndex][DataIndex];
			}

			T& operator*()
			{
				return Pointer[ChunkIndex][DataIndex];
			}

			bool operator==(const Iterator& Other) const
			{
				return Pointer == Other.Pointer && ChunkIndex == Other.ChunkIndex && DataIndex == Other.DataIndex;
			}

			bool operator!=(const Iterator& Other) const
			{
				return Pointer != Other.Pointer || ChunkIndex != Other.ChunkIndex || DataIndex != Other.DataIndex;
			}

		private:
			T** Pointer;
			uint64 ChunkIndex;
			uint64 DataIndex;
		};

		Dequeue(Allocator* Alloc = nullptr)
			: Allocator(nullptr), Chunks(1), Count(0), Front(5), Back(4), Data(nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
			Data = (T**)Memory::Allocate(sizeof(T*) * Chunks, NEXUS_MEMORY_ALIGN, Allocator);
			Data[0] = (T*)Memory::Allocate(sizeof(T) * ChunkSize, NEXUS_MEMORY_ALIGN, Allocator);
		}

		Dequeue(const Dequeue<T>& Other)
			: Allocator(Other.Allocator), Chunks(Other.Chunks), Count(Other.Count), Front(Other.Front), Back(Other.Back), Data(Other.Data)
		{
		}

		Dequeue(Dequeue<T>&& Other) noexcept
			: Allocator(Other.Allocator), Chunks(Other.Chunks), Count(Other.Count), Front(Other.Front), Back(Other.Back), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Dequeue()
		{
			for (uint64 Index = 0; Index < Chunks; Index++)
			{
				Memory::Free(Data[Index], Allocator);
			}

			Memory::Free(Data, Allocator);
		}

		Dequeue<T> Copy() const
		{
			Dequeue<T> Copy = Dequeue(Allocator);
			
			Copy.Chunks = Chunks;
			Copy.Count = Count;
			Copy.Front = Front;
			Copy.Back = Back;

			Copy.ResizeChunks();
			for (uint64 C = 1; C < Chunks; C++)
			{
				Copy.CreateChunk(C);
			}

			for (uint64 C = 0; C < Chunks; C++)
			{
				for (uint64 I = 0; I < ChunkSize; I++)
				{
					Copy.Data[C][I] = Data[C][I];
				}
			}

			return Copy;
		}

		T& operator[](uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			return Data[ChunkIndex][DataIndex];
		}

		const T& operator[](uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			return Data[ChunkIndex][DataIndex];
		}

		bool operator==(const Dequeue<T>& Other)
		{
			return Chunks == Other.Chunks && Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dequeue<T>& Other)
		{
			return Chunks != Other.Chunks || Count != Other.Count || Data != Other.Data;
		}

		void Assign(uint64 Index, const T& Value)
		{
			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			Data[ChunkIndex][DataIndex] = Value;
		}

		void Assign(uint64 Index, T&& Value)
		{
			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			Data[ChunkIndex][DataIndex] = Move(Value);
		}

		template<typename... Args>
		void Assign(uint64 Index, Args&&... args)
		{
			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			Data[ChunkIndex][DataIndex] = T(args...);
		}

		void Assign(uint64 Index, const Dequeue<T>& Value)
		{
			NEXUS_ASSERT(IsValidIndex(Index + Value.Count), "Overflow");

			for (uint64 Offset = 0; Offset < Value.Count; Offset++)
			{
				Assign(Index + Offset, Value[Offset]);
			}
		}

		void AppendBack(const T& Value)
		{
			Grow(true);
			Data[Chunks - 1][Back] = Value;
		}

		void AppendBack(T&& Value)
		{
			Grow(true);
			Data[Chunks - 1][Back] = Move(Value);
		}

		template<typename... Args>
		void AppendBack(Args&&... args)
		{
			Grow(true);
			Data[Chunks - 1][Back] = T(args...);
		}

		void AppendBack(const Dequeue<T>& Value)
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
		void AppendFront(Args&&... args)
		{
			Grow(false);
			Data[0][Front] = T(args...);
		}

		void AppendFront(const Dequeue<T>& Value)
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
			for (uint64 Index = 0; Index < Chunks; Index++)
			{
				Memory::Free(Data[Index], Allocator);
			}

			Chunks = 1;
			Count = 0;
			Front = 5;
			Back = 4;

			ResizeChunks();
			CreateChunk(0);
		}

		T& Get(uint64 Index)
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			return Data[ChunkIndex][DataIndex];
		}

		const T& Get(uint64 Index) const
		{
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");

			uint64 ChunkIndex, DataIndex;
			ConvertIndex(Index, ChunkIndex, DataIndex);
			return Data[ChunkIndex][DataIndex];
		}

		T& First()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			return Data[0][Front];
		}

		T& Last()
		{
			NEXUS_ASSERT(!IsEmpty(), "Dequeue is Empty");
			
			return Data[Chunks - 1][Back];
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
			Iterator It = Iterator(Data, Chunks - 1, Back);
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
				uint64 ChunkIndex, DataIndex;
				ConvertIndex(Index, ChunkIndex, DataIndex);
				if (Data[ChunkIndex][DataIndex] == Other)
				{
					return Index;
				}
			}

			return Count;
		}

		uint64 GetCount() const { return Count; }
		uint64 GetChunks() const { return Chunks; }
		bool IsEmpty() const { return Count == 0; }

	private:
		void CreateChunk(uint64 Index)
		{
			Data[Index] = (T*)Memory::Allocate(sizeof(T) * ChunkSize, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void DestroyChunk(uint64 Index)
		{
			Memory::Free(Data[Index], Allocator);
		}

		void ResizeChunks()
		{
			Data = (T**)Memory::Realloc(Data, sizeof(T*) * Chunks, NEXUS_MEMORY_ALIGN, Allocator);
		}

		void MoveChunks(bool Forward)
		{
			T** Start = Forward ? &Data[0] : &Data[1];
			T** End = Forward ? &Data[1] : &Data[0];
			Memory::MemCopy(Start, End, sizeof(T*) * (Chunks - 1));
		}

		void ConvertIndex(uint64 Index, uint64& ChunkIndex, uint64& DataIndex) const
		{
			Index += Front;
			ChunkIndex = Index / ChunkSize;
			DataIndex = Index % ChunkSize;
		}

		void Grow(bool AppendBack)
		{
			Count++;

			if ((AppendBack && Back == ChunkSize - 1) || (!AppendBack && Front == 0))
			{
				Chunks++;
				ResizeChunks();
				if (!AppendBack)
				{
					MoveChunks(true);
				}
				CreateChunk(AppendBack ? Chunks - 1 : 0);

				if (AppendBack)
				{
					Back = 0;
				}
				else
				{
					Front = ChunkSize - 1;
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

			if ((RemoveBack && Back == 0) || (!RemoveBack && Front == ChunkSize - 1))
			{
				DestroyChunk(RemoveBack ? Chunks - 1 : 0);
				if (!RemoveBack)
				{
					MoveChunks(false);
				}
				Chunks--;
				ResizeChunks();

				if (RemoveBack)
				{
					Back = ChunkSize - 1;
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

		static const uint64 ChunkSize = 10;

		Allocator* Allocator;
		uint64 Chunks;
		uint64 Count;
		uint64 Front;
		uint64 Back;
		T** Data;
	};
}