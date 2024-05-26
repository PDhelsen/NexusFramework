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
			Node* Instance = CreateNode();
			Instance->Data = Value;
			AppendNode(Instance);
		}

		void Append(T&& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Move(Value);
			AppendNode(Instance);
		}

		template<typename... Args>
		void AppendConstruct(Args&&... args)
		{
			Node* Instance = CreateNode();
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
			Node* Instance = RemoveNode();
			DestroyNode(Instance);
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
			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Next = nullptr;
			return Instance;
		}

		void DestroyNode(Node* Instance)
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
			Head = Head->Next;
			return Instance;
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Head;
		Node* Tail;
	};
}