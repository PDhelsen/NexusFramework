#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class LinkedList
	{
		struct Node
		{
			T Data;
			Node* Next;
			Node* Prev;
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
				Pointer = &(NodeFromData(Pointer)->Next->Data);
				return *this;
			}

			Iterator operator++(int32)
			{
				Iterator Copy = *this;
				++(*this);
				return Copy;
			}

			Iterator& operator--()
			{
				Pointer = &(NodeFromData(Pointer)->Prev->Data);
				return *this;
			}

			Iterator operator--(int32)
			{
				Iterator Copy = *this;
				--(*this);
				return Copy;
			}

			T* operator->()
			{
				return Pointer;
			}

			T& operator*()
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

		LinkedList(Allocator* Alloc = nullptr)
			: Allocator(Alloc), Count(0), Head(nullptr), Tail(nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
		}

		LinkedList(const LinkedList<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Tail(Other.Tail)
		{
		}

		LinkedList(LinkedList<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Head(Other.Head), Tail(Other.Tail)
		{
			Other.Head = nullptr;
			Other.Tail = nullptr;
		}

		~LinkedList()
		{
			Clear();
		}

		LinkedList<T> Copy() const
		{
			LinkedList Copy = LinkedList(Allocator);

			Node* Current = Head;
			while (Current != nullptr)
			{
				Copy.AppendBack(Current->Data);
				Current = Current->Next;
			}

			return Copy;
		}

		bool operator==(const LinkedList<T>& Other)
		{
			return Count == Other.Count && Head == Other.Head && Tail == Other.Tail;
		}

		bool operator!=(const LinkedList<T>& Other)
		{
			return Count != Other.Count || Head != Other.Head || Tail != Other.Tail;
		}

		void Assign(T* Index, const T& Value)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");

			Node* Instance = NodeFromData(Index);
			Instance->Data = Value;
		}

		void Assign(T* Index, T&& Value)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* Instance = NodeFromData(Index);
			Instance->Data = Move(Value);
		}

		template<typename... Args>
		void Assign(T* Index, Args&&... args)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* Instance = NodeFromData(Index);
			Memory::Construct<T>(&Instance->Data, args...);
		}

		void AppendBack(const T& Value)
		{
			Node* New = CreateNode();
			New->Data = Value;

			AddToTail(New);
		}

		void AppendBack(T&& Value)
		{
			Node* New = CreateNode();
			New->Data = Move(Value);

			AddToTail(New);
		}

		template<typename... Args>
		void AppendBack(Args&&... args)
		{
			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			AddToTail(New);
		}

		void AppendBack(const LinkedList<T>& Other)
		{
			Node* New = Other.Head;
			while (New != nullptr)
			{
				AppendBack(New->Data);
				New = New->Next;
			}
		}

		void AppendFront(const T& Value)
		{
			Node* New = CreateNode();
			New->Data = Value;

			AddToHead(New);
		}

		void AppendFront(T&& Value)
		{
			Node* New = CreateNode();
			New->Data = Move(Value);

			AddToHead(New);
		}

		template<typename... Args>
		void AppendFront(Args&&... args)
		{
			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			AddToHead(New);
		}

		void AppendFront(const LinkedList<T>& Other)
		{
			Node* New = Other.Head;
			while (New != nullptr)
			{
				AppendFront(New->Data);
				New = New->Next;
			}
		}

		void InsertAfter(T* Index, const T& Value)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = CreateNode();
			New->Data = Value;

			Node* Instance = NodeFromData(Index);
			InsertAfter(Instance, New);
		}

		void InsertAfter(T* Index, T&& Value)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = CreateNode();
			New->Data = Move(Value);

			Node* Instance = NodeFromData(Index);
			InsertAfter(Instance, New);
		}

		template<typename... Args>
		void InsertAfter(T* Index, Args&&... args)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			Node* Instance = NodeFromData(Index);
			InsertAfter(Instance, New);
		}

		void InsertAfter(T* Index, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = Other.Head;
			Node* Instance = NodeFromData(Index);

			while (New != nullptr)
			{
				InsertAfter(Instance, New->Data);
				New = New->Next;
				Instance = Instance->Next;
			}
		}

		void InsertBefore(T* Index, const T& Value)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = CreateNode();
			New->Data = Value;

			Node* Instance = NodeFromData(Index);
			InsertBefore(Instance, New);
		}

		void InsertBefore(T* Index, T&& Value)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = CreateNode();
			New->Data = Move(Value);

			Node* Instance = NodeFromData(Index);
			InsertBefore(Instance, New);
		}

		template<typename... Args>
		void InsertBefore(T* Index, Args&&... args)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			Node* Instance = NodeFromData(Index);
			InsertBefore(Instance, New);
		}

		void InsertBefore(T* Index, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* New = Other.Head;
			Node* Instance = NodeFromData(Index);

			while (New != nullptr)
			{
				InsertBefore(Instance, New->Data);
				New = New->Next;
				Instance = Instance->Next;
			}
		}

		void RemoveBack()
		{
			Node* Removed = RemoveFromTail();
			DestroyNode(Removed);
		}

		void RemoveBack(T* Index)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* Target = NodeFromData(Index);
			while (Tail != Target)
			{
				RemoveBack();
			}
			RemoveBack();
		}

		void RemoveFront()
		{
			Node* Removed = RemoveFromHead();
			DestroyNode(Removed);
		}

		void RemoveFront(T* Index)
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");
			
			Node* Target = NodeFromData(Index);
			while (Head != Target)
			{
				RemoveFront();
			}
			RemoveFront();
		}

		void Remove(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			
			Node* Start = NodeFromData(From);
			Node* End = NodeFromData(To);
		
			Node* Before = Start->Prev;
			Node* After = End->Next;

			while (Start != End)
			{
				Node* Removed = Start;
				Start = Start->Next;
				DestroyNode(Removed);
			}

			DestroyNode(End);

			if (Before)
			{
				Before->Next = After;
			}
			else
			{
				Head = After;
			}

			if (After)
			{
				After->Prev = Before;
			}
			else
			{
				Tail = Before;
			}
		}

		void Clear()
		{
			while(Tail)
			{
				RemoveBack();
			}
		}

		T& GetNext(T* Index) const
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");

			Node* Instance = NodeFromData(Index);
			NEXUS_ASSERT(Instance != Tail, "Cannot get next on Tail");

			return Instance->Next->Data;
		}

		T* TryGetNext(T* Index) const
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");

			Node* Instance = NodeFromData(Index);
			if (Instance == Tail)
			{
				return nullptr;
			}

			return &Instance->Next->Data;
		}

		T& GetPrev(T* Index) const
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");

			Node* Instance = NodeFromData(Index);
			NEXUS_ASSERT(Instance != Head, "Cannot get prev on Head");

			return Instance->Prev->Data;
		}

		T* TryGetPrev(T* Index) const
		{
			NEXUS_ASSERT(Index != nullptr, "Index is null");

			Node* Instance = NodeFromData(Index);
			if (Instance == Head)
			{
				return nullptr;
			}

			return &Instance->Prev->Data;
		}

		T& First() const
		{
			return Head->Data;
		}

		T& Last() const
		{
			return Tail->Data;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&Head->Data);
		}

		Iterator BeginReverse() const
		{
			return Iterator(&Tail->Data);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(nullptr);
		}

		Iterator EndReverse() const
		{
			return Iterator(nullptr);
		}

		void Swap(T* A, T* B)
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			Node* NodeA = NodeFromData(A);
			Node* NodeB = NodeFromData(B);

			Node* NextA = NodeA->Next;
			Node* PrevA = NodeA->Prev;
			Node* NextB = NodeB->Next;
			Node* PrevB = NodeB->Prev;

			if (NextA == NodeB || PrevA == NodeB || NextB == NodeA || PrevB == NodeA)
			{
				NodeA->Next = NextB;
				NodeB->Prev = PrevA;
				NodeB->Next = NodeA;
				NodeA->Prev = NodeB;

				if (PrevA)
				{
					PrevA->Next = NodeB;
				}
				if (NextB)
				{
					NextB->Prev = NodeA;
				}
			}
			else
			{
				NodeA->Next = NextB;
				NodeA->Prev = PrevB;
				NodeB->Next = NextA;
				NodeB->Prev = PrevA;

				if (NextA)
				{
					NextA->Prev = NodeB;
				}
				if (PrevA)
				{
					PrevA->Next = NodeB;
				}
				if (NextB)
				{
					NextB->Prev = NodeA;
				}
				if (PrevB)
				{
					PrevB->Next = NodeA;
				}
			}

			if (NodeA == Head)
			{
				Head = NodeB;
			}
			else if (NodeB == Head)
			{
				Head = NodeA;
			}
			if (NodeA == Tail)
			{
				Tail = NodeB;
			}
			else if (NodeB == Tail)
			{
				Tail = NodeA;
			}
		}

		bool Contains(const T& Other) const
		{
			Node* Node = Head;
			while (Node != nullptr)
			{
				if (Node->Data == Other)
				{
					return true;
				}

				Node = Node->Next;
			}

			return false;
		}

		void Sort()
		{
			SortSort(&Head);
			FixLinks();
		}

		void Reverse()
		{
			uint64 Half = Count / 2;
			Node* A = Head;
			Node* B = Tail;
			for (uint64 It = 0; It < Half; It++)
			{
				Node* Next = A->Next;
				Node* Prev = B->Prev;
				
				Swap(&A->Data, &B->Data);

				A = Next;
				B = Prev;
			}
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

		void AddToTail(Node* New)
		{
			New->Next = nullptr;
			New->Prev = Tail;

			if (!Head)
			{
				Head = New;
			}

			if (Tail)
			{
				Tail->Next = New;
			}

			Tail = New;
		}

		void AddToHead(Node* New)
		{
			New->Next = Head;
			New->Prev = nullptr;

			if (!Tail)
			{
				Tail = New;
			}

			if (Head)
			{
				Head->Prev = New;
			}

			Head = New;
		}

		void InsertAfter(Node* Instance, Node* New)
		{
			New->Next = Instance->Next;
			New->Prev = Instance;
			Instance->Next->Prev = New;
			Instance->Next = New;
		}

		void InsertBefore(Node* Instance, Node* New)
		{
			New->Prev = Instance->Prev;
			New->Next = Instance;
			Instance->Prev->Next= New;
			Instance->Prev = New;
		}

		Node* RemoveFromTail()
		{
			Node* Removed = Tail;
			Tail = Tail->Prev;

			if (Tail)
			{
				Tail->Next = nullptr;
			}
			
			if (Head == Removed)
			{
				Head = nullptr;
			}

			return Removed;
		}

		Node* RemoveFromHead()
		{
			Node* Removed = Head;
			Head = Head->Next;

			if (Head)
			{
				Head->Prev = nullptr;
			}

			if (Tail == Removed)
			{
				Tail = nullptr;
			}

			return Removed;
		}

		static void SortSort(Node** HeadRef)
		{
			Node* Head = *HeadRef;	
			if (Head == nullptr || Head->Next == nullptr)
			{
				return;
			}

			Node* A = nullptr;
			Node* B = nullptr;
			SortSplit(Head, &A, &B);

			SortSort(&A);
			SortSort(&B);

			*HeadRef = SortMerge(A, B);
		}

		static Node* SortMerge(Node* A, Node* B)
		{
			Node* Result = nullptr;

			if (A == nullptr)
			{
				return B;
			}
			else if (B == nullptr)
			{
				return A;
			}

			if (A->Data <= B->Data)
			{
				Result = A;
				Result->Next = SortMerge(A->Next, B);
			}
			else
			{
				Result = B;
				Result->Next = SortMerge(A, B->Next);
			}

			return Result;
		}

		static void SortSplit(Node* Source, Node** FrontRef, Node** BackRef)
		{
			Node* Slow;
			Node* Fast;

			Slow = Source;
			Fast = Source->Next;

			while (Fast != nullptr)
			{
				Fast = Fast->Next;
				if (Fast != nullptr)
				{
					Slow = Slow->Next;
					Fast = Fast->Next;
				}
			}

			*FrontRef = Source;
			*BackRef = Slow->Next;

			Slow->Next = nullptr;
		}

		void FixLinks()
		{
			Node* Current = Head;
			Current->Prev = nullptr;

			while (Current != nullptr)
			{
				Node* Prev = Current;
				Current = Current->Next;

				if (Current)
				{
					Current->Prev = Prev;
				}
				else
				{
					Prev->Next = nullptr;
					Tail = Prev;
				}
			}
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Head;
		Node* Tail;
	};
}