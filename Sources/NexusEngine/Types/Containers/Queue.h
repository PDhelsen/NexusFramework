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
		Queue(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Head(nullptr), Tail(nullptr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Queue(const Queue<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Tail(Other.Tail)
		{
		}

		Queue(Queue<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Tail(Other.Tail)
		{
			Other.Head = nullptr;
			Other.Tail = nullptr;
		}

		~Queue()
		{
			Clear();
		}

		Queue<T> Copy() const
		{
			Queue<T> Copy = Queue<T>(Allocator);
			Node* Current = Head;
			while (Current)
			{
				Copy.Append(Current->Data);
				Current = Current->Next;
			}
			return Copy;
		}

		bool operator==(const Queue<T>& Other) const
		{
			return Count == Other.Count && Head == Other.Head && Tail == Other.Tail;
		}

		bool operator!=(const Queue<T>& Other) const
		{
			return Count != Other.Count || Head != Other.Head || Tail != Other.Tail;
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
			Node* Current = Value.Head;
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
			while (Head)
			{
				Remove();
			}
		}

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Queue is empty");
			
			return Head->Data;
		}

		bool Contains(const T& Other) const
		{
			Node* Current = Head;
			while (Current)
			{
				if (Current->Data == Other)
				{
					return true;
				}

				Current = Current->Next;
			}

			return false;
		}

		void Reverse()
		{
			Tail = Head;

			Node* Current = Head;
			Node* Next = Current->Next;
			Current->Next = nullptr;

			while (Next)
			{
				Node* SecondNext = Next->Next;
				Next->Next = Current;

				Current = Next;
				Next = SecondNext;
			}

			Head = Current;
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
			if (Head == nullptr || Tail == nullptr)
			{
				Head = Tail = Instance;
				return;
			}

			Tail->Next = Instance;
			Tail = Instance;
		}

		Node* RemoveNode()
		{
			Node* Instance = Head;
			if (Head)
			{
				Head = Head->Next;
			}
			if (Head == nullptr)
			{
				Tail = nullptr;
			}
			return Instance;
		}

		static Node* GetNode(T* Data)
		{
			return reinterpret_cast<Node*>(Data);
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Head;
		Node* Tail;
	};
}