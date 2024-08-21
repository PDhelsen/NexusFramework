#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"
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
		using Node = LinkedNodeSimple<T>;

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

		Pool(const Pool<T>& Other) = delete;

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

		Pool<T>& operator=(const Pool<T>& Other)
		{
			if (*this == Other)
			{
				return *this;
			}

			Allocator = Other.Allocator;
			Count = Other.Count;
			Data = Other.Data;
			Own = Other.Own;

			return *this;
		}

		Pool<T>& operator=(Pool<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Allocator = Other.Allocator;
			Count = Other.Count;
			Data = Other.Data;
			Own = Other.Own;

			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
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
				Construct(Instance);
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
				Destruct(Data);
				Free(Data);
				Data = Next;
			}

			Count = 0;
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
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
			Memory::Free(Instance, Allocator);
		}

		void Construct(Node* Instance)
		{
			Memory::Construct<T>(&Instance->Value);
		}

		void Destruct(Node* Instance)
		{
			Memory::Destruct(&Instance->Value);
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
