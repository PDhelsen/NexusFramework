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
			T Data;
			Node* Next;
		};

	public:
		Pool(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Head(nullptr), Own(false)
		{
			NEXUS_ASSERT(!IsPointer<T>::Check(), "Pool of pointer type is not allowed !");

			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Pool(uint64 Size)
			: Allocator(nullptr), Count(0), Head(nullptr), Own(true)
		{
			NEXUS_ASSERT(!IsPointer<T>::Check(), "Pool of pointer type is not allowed !");

			AllocatorActive RawAllocator(nullptr);
			Allocator = new PoolAllocator(Size, sizeof(Node));
		}

		Pool(const Pool<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Own(Other.Own)
		{
		}

		Pool(Pool<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Own(Other.Own)
		{
			Other.Head = nullptr;

			if (Other.Own)
			{
				Other.Allocator = nullptr;
			}
		}

		~Pool()
		{
			Clear();

			if (Own)
			{
				AllocatorActive RawAllocator(nullptr);
				delete (PoolAllocator*)Allocator;
			}
		}

		bool operator==(const Pool<T>& Other) const
		{
			return Count == Other.Count && Head == Other.Head && Own == Other.Own;
		}

		bool operator!=(const Pool<T>& Other) const
		{
			return Count != Other.Count || Head != Other.Head || Own != Other.Own;
		}

		T& Acquire()
		{
			T* Instance = nullptr;

			if (IsEmpty())
			{
				Node* New = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
				Memory::Construct<T>(&New->Data);
				Instance = &New->Data;
			}
			else
			{
				Node* Old = Head;
				Head = Head->Next;
				Instance = &Old->Data;

				Count--;
			}

			return *Instance;
		}

		void Recycle(T& Instance)
		{
			NEXUS_ASSERT(Allocator->IsValidAddress(&Instance), "Object was not allocated from the pool");

			Node* N = reinterpret_cast<Node*>(&Instance);
			
			N->Next = Head;
			Head = N;

			Count++;
		}

		void Clear()
		{
			while (Head)
			{
				Node* Next = Head->Next;

				Memory::Destruct<T>(&Head->Data);
				Memory::Free(Head, Allocator);
				
				Head = Next;
			}

			Count = 0;
		}

		uint64 GetCount() const { return Count; }
		bool IsEmpty() const { return Count == 0; }
		bool OwnAllocator() const { return Own; }

	private:
		Allocator* Allocator;
		uint64 Count;
		Node* Head;
		bool Own;
	};
}