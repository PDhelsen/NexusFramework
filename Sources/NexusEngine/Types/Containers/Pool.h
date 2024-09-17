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
		using N = Node::NodeSimple<T>;

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
		Pool(Pool<T>&& Other) noexcept = delete;

		~Pool()
		{
			Clear();
			Free();
		}

		Pool<T>& operator=(const Pool<T>& Other) = delete;
		Pool<T>& operator=(Pool<T>&& Other) noexcept = delete;

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
				N* Instance = Allocate();
				Construct(Instance);
				Value = &Instance->Value;
			}
			else
			{
				N* Instance = Data;
				Data = Data->Next;
				Value = &Instance->Value;

				--Count;
			}

			return *Value;
		}

		void Recycle(T& Value)
		{
			N* Instance = reinterpret_cast<N*>(&Value);
			
			Instance->Next = Data;
			Data = Instance;

			++Count;
		}

		void Clear()
		{
			while (Data)
			{
				N* Next = Data->Next;
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
			Allocator = new PoolAllocator(Size, sizeof(N));
		}

		N* Allocate()
		{
			N* Instance = (N*)Memory::Allocate(sizeof(N), NEXUS_MEMORY_ALIGN, Allocator);
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

		void Free(N* Instance)
		{
			Memory::Free(Instance, Allocator);
		}

		void Construct(N* Instance)
		{
			Memory::Construct<T>(&Instance->Value);
		}

		void Destruct(N* Instance)
		{
			Memory::Destruct(&Instance->Value);
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Allocator;
		uint64 Count;
		N* Data;
		bool Own;
	};
}
