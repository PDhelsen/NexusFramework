#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/PoolAllocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Pool
	{
	public:
		Pool(uint64 Cpct = 2, Allocator* AllocStructure = nullptr, Allocator* AllocData = nullptr)
			: AllocatorStructure(nullptr), AllocatorData(nullptr), Capacity(0), Count(0), OwnAllocator(false), Data(nullptr)
		{
			AllocatorStructure = AllocStructure != nullptr ? AllocStructure : Memory::GetActiveAllocator();
			AllocatorData = AllocData != nullptr ? AllocData : Memory::GetActiveAllocator();
			Capacity = GetValidCapacity(Cpct);
			Data = (T**)Memory::Allocate(sizeof(T*) * Capacity, NEXUS_MEMORY_ALIGN, AllocatorStructure);
		}

		Pool(uint64 Cpct, Allocator* AllocStructure = nullptr, bool AllocData = false)
			: AllocatorStructure(nullptr), AllocatorData(nullptr), Capacity(0), Count(0), OwnAllocator(AllocData), Data(nullptr)
		{
			AllocatorStructure = AllocStructure != nullptr ? AllocStructure : Memory::GetActiveAllocator();
			AllocatorData = AllocData ? new PoolAllocator(Cpct, sizeof(T)) : Memory::GetActiveAllocator();
			Capacity = GetValidCapacity(Cpct);
			Data = (T**)Memory::Allocate(sizeof(T*) * Capacity, NEXUS_MEMORY_ALIGN, AllocatorStructure);
		}

		Pool(const Pool<T>& Other)
			: AllocatorStructure(Other.AllocStructure), AllocatorData(Other.AllocatorData), Capacity(Other.Capacity), Count(Other.Count), OwnAllocator(Other.OwnAllocator), Data(Other.Data)
		{
		}

		Pool(Pool<T>&& Other) noexcept
			: AllocatorStructure(Other.AllocStructure), AllocatorData(Other.AllocatorData), Capacity(Other.Capacity), Count(Other.Count), OwnAllocator(Other.OwnAllocator), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Pool()
		{
			Clear();
			if (OwnAllocator)
			{
				delete (PoolAllocator*)AllocatorData;
			}

			Memory::Free(Data, AllocatorStructure);
		}

		bool operator==(const Pool<T> Other)
		{
			return Count == Other.Count && OwnAllocator == Other.OwnAllocator && Data == Other.Data;
		}

		bool operator!=(const Pool<T> Other)
		{
			return Count != Other.Count || OwnAllocator != Other.OwnAllocator || Data != Other.Data;
		}

		T* Acquire()
		{
			T* Instance = nullptr;

			if (IsEmpty())
			{
				Instance = (T*)Memory::Allocate(sizeof(T), NEXUS_MEMORY_ALIGN, AllocatorData);
				Memory::Construct<T>(Instance);
			}
			else
			{
				Instance = Data[Count - 1];
				Resize(--Count);
			}

			return Instance;
		}

		void Recycle(T* Instance)
		{
			Resize(++Count);
			Data[Count - 1] = Instance;
		}

		void Clear(bool ShrinkToZero = false)
		{
			for (uint64 Index = 0; Index < Count; Index++)
			{
				Memory::Destruct(Data[Index]);
				Memory::Free(Data[Index], AllocatorData);
			}

			Resize(0);
			if (ShrinkToZero)
			{
				Shrink();
			}
		}

		void Reserve(uint64 Size)
		{
			NEXUS_ASSERT(!OwnAllocator, "Cannot modify capacity if using own allactor");

			if (Size <= Capacity)
			{
				return;
			}

			Capacity = GetValidCapacity(Size);
			Data = (T**)Memory::Realloc(Data, sizeof(T*) * Capacity, NEXUS_MEMORY_ALIGN, AllocatorStructure);
		}

		void Shrink()
		{
			NEXUS_ASSERT(!OwnAllocator, "Cannot modify capacity if using own allactor");
			
			Capacity = GetValidCapacity(Count);
			Data = (T**)Memory::Realloc(Data, sizeof(T*) * Capacity, NEXUS_MEMORY_ALIGN, AllocatorStructure);
		}

		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }
		bool IsEmpty() const { return Count == 0; }

	private:
		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				NEXUS_ASSERT(!OwnAllocator, "Cannot modify capacity if using own allactor");

				uint64 NewCapacity = Capacity + Capacity / 2;
				Capacity = GetValidCapacity(NewCapacity > Count ? NewCapacity : Count);

				Data = (T**)Memory::Realloc(Data, sizeof(T*) * Capacity, NEXUS_MEMORY_ALIGN, AllocatorStructure);
			}

			NEXUS_ASSERT(Count <= Capacity, "Overflowing list");
		}

		uint64 GetValidCapacity(uint64 Size) { return Size > 2 ? Size : 2; }

		Allocator* AllocatorStructure;
		Allocator* AllocatorData;
		uint64 Capacity;
		uint64 Count;
		bool OwnAllocator;
		T** Data;
	};
}