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
		struct Node
		{
			T Value;
			Node* Next;
		};

	public:
		Pool(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Data(nullptr), Own(false)
		{
			NEXUS_ASSERT(!IsPointer<T>::Check(), "Pool of pointer type is not allowed !");

			ValidateAllocator(Allctr);
		}

		Pool(uint64 Size)
			: Allocator(nullptr), Count(0), Data(nullptr), Own(true)
		{
			NEXUS_ASSERT(!IsPointer<T>::Check(), "Pool of pointer type is not allowed !");

			Allocate(Size);
		}

		Pool(const Pool<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data), Own(Other.Own)
		{
		}

		Pool(Pool<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data), Own(Other.Own)
		{
			Other.Data = nullptr;

			if (Other.Own)
			{
				Other.Allocator = nullptr;
			}
		}

		~Pool()
		{
			Clear();
			Free();
		}

		bool operator==(const Pool<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Pool<T>& Other) const
		{
			return !(*this == Other);
		}

		T& Acquire()
		{
			T* Value = nullptr;

			if (IsEmpty())
			{
				Node* Instance = Allocate();
				Value = &Instance->Value;
			}
			else
			{
				Node* Instance = Data;
				Data = Data->Next;
				Value = &Instance->Value;

				Count--;
			}

			return *Value;
		}

		void Recycle(T& Value)
		{
			Node* Instance = reinterpret_cast<Node*>(&Value);
			
			Instance->Next = Data;
			Data = Instance;

			Count++;
		}

		void Clear()
		{
			while (Data)
			{
				Node* Next = Data->Next;
				Free(Data);
				Data = Next;
			}

			Count = 0;
		}

		uint64 GetCount() const { return Count; }
		bool IsEmpty() const { return Count == 0; }
		bool OwnAllocator() const { return Own; }

	private:
		void Allocate(uint64 Size)
		{
			AllocatorActive RawAllocator(nullptr);
			Allocator = new PoolAllocator(Size, sizeof(Node));
		}

		Node* Allocate()
		{
			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Next = nullptr;
			Memory::Construct<T>(&Instance->Value);
			return Instance;
		}

		void Free()
		{
			if (Own)
			{
				AllocatorActive RawAllocator(nullptr);
				delete (PoolAllocator*)Allocator;
			}
		}

		void Free(Node* Instance)
		{
			Memory::Destruct<T>(&Instance->Value);
			Memory::Free(Instance, Allocator);
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Data;
		bool Own;
	};
}