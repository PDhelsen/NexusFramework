#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Queue
	{
		struct Node
		{
			T Data;
			Node* Next;
		};

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
				Pointer = &(GetNode(Pointer)->Next->Data);
				return *this;
			}

			Iterator operator++(int32)
			{
				Iterator Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return Pointer;
			}

			T& operator*() const
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

		Queue(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Queue(const Queue<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
		{
		}

		Queue(Queue<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
		{
			Other.DataHead = nullptr;
			Other.DataTail = nullptr;
		}

		~Queue()
		{
			Clear();
		}

		Queue<T> Copy() const
		{
			Queue<T> Copy = Queue<T>(Allocator);

			Node* Current = DataHead;
			while (Current)
			{
				Copy.Append(Current->Data);
				Current = Current->Next;
			}

			return Copy;
		}

		bool operator==(const Queue<T>& Other) const
		{
			return Count == Other.Count && DataHead == Other.DataHead && DataTail == Other.DataTail;
		}

		bool operator!=(const Queue<T>& Other) const
		{
			return !(*this == Other);
		}

		void Append(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Value;

			AppendNode(Instance);
		}

		void Append(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Move(Value);

			AppendNode(Instance);
		}

		template<typename... Args>
		void AppendConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Data, args...);

			AppendNode(Instance);
		}

		void AppendRange(const Queue<T>& Value)
		{
			Node* Current = Value.DataHead;
			while (Current)
			{
				Append(Current->Data);
				Current = Current->Next;
			}
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), "Queue is empty");
			
			Node* Instance = RemoveNode();
			Free(Instance);
		}

		void Clear()
		{
			while (DataHead)
			{
				Remove();
			}
		}

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Queue is empty");
			
			return DataHead->Data;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&DataHead->Data);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(nullptr);
		}

		void Swap(T* A, T* B)
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			T Temp = GetNode(A)->Data;
			GetNode(A)->Data = Move(GetNode(B)->Data);
			GetNode(B)->Data = Move(Temp);
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != nullptr;
		}

		T* Find(const T& Other) const
		{
			Node* Current = DataHead;
			while (Current != nullptr)
			{
				if (Current->Data == Other)
				{
					return &Current->Data;
				}

				Current = Current->Next;
			}

			return nullptr;
		}

		void Reverse()
		{
			DataTail = DataHead;

			Node* Current = DataHead;
			Node* Next = Current->Next;
			Current->Next = nullptr;

			while (Next)
			{
				Node* SecondNext = Next->Next;
				Next->Next = Current;

				Current = Next;
				Next = SecondNext;
			}

			DataHead = Current;
		}

		uint64 GetCount() const { return Count; }
		bool IsEmpty() const { return Count == 0; }

	private:
		Node* Allocate()
		{
			Count++;

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Next = nullptr;
			return Instance;
		}

		void Free(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* Instance)
		{
			if (DataHead == nullptr || DataTail == nullptr)
			{
				DataHead = DataTail = Instance;
				return;
			}

			DataTail->Next = Instance;
			DataTail = Instance;
		}

		Node* RemoveNode()
		{
			Node* Instance = DataHead;
			if (DataHead)
			{
				DataHead = DataHead->Next;
			}
			if (DataHead == nullptr)
			{
				DataTail = nullptr;
			}
			return Instance;
		}

		static Node* GetNode(T* Data)
		{
			return reinterpret_cast<Node*>(Data);
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Allocator;
		uint64 Count;
		Node* DataHead;
		Node* DataTail;
	};
}