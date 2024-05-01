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
		Queue(Allocator* Alloc = nullptr)
			: Allocator(nullptr), Count(0), Head(nullptr), Tail(nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
		}

		Queue(const Queue<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Tail(Other.Tail)
		{
		}

		Queue(Queue<T>&& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Tail(Other.Tail)
		{
			Other.Head = nullptr;
			Other.Tail = nullptr;
		}

		~Queue()
		{
			Clear();
		}

		Queue<T> Copy()
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

		bool operator==(const Queue<T>& Other)
		{
			return Count == Other.Count && Head == Other.Head && Tail == Other.Tail;
		}

		bool operator!=(const Queue<T>& Other)
		{
			return Count != Other.Count || Head != Other.Head || Tail != Other.Tail;
		}

		void Append(const T& Value)
		{
			Node* New = CreateNode();
			New->Data = Value;
			AppendNode(New);
		}

		void Append(T&& Value)
		{
			Node* New = CreateNode();
			New->Data = Move(Value);
			AppendNode(New);
		}

		template<typename... Args>
		void Append(Args&&... args)
		{
			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);
			AppendNode(New);
		}

		void Append(const Queue<T>& Value)
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
			Node* Removed = RemoveNode();
			DestroyNode(Removed);
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
		static Node* NodeFromData(T* Data)
		{
			return reinterpret_cast<Node*>(Data);
		}

		Node* CreateNode()
		{
			Count++;
			return (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
		}

		void DestroyNode(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* New)
		{
			if (Head == nullptr || Tail == nullptr)
			{
				Head = Tail = New;
				return;
			}

			Tail->Next = New;
			Tail = New;
		}

		Node* RemoveNode()
		{
			Node* Removed = Head;
			Head = Head->Next;
			return Removed;
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Head;
		Node* Tail;
	};
}