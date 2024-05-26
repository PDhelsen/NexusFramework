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
				Iterator Temp = *this;
				++(*this);
				return Temp;
			}

			Iterator& operator--()
			{
				Pointer = &(NodeFromData(Pointer)->Prev->Data);
				return *this;
			}

			Iterator operator--(int32)
			{
				Iterator Temp = *this;
				--(*this);
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

		LinkedList(Allocator* Allctr = nullptr)
			: Allocator(Allctr), Count(0), Head(nullptr), Tail(nullptr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
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

		bool operator==(const LinkedList<T>& Other) const
		{
			return Count == Other.Count && Head == Other.Head && Tail == Other.Tail;
		}

		bool operator!=(const LinkedList<T>& Other) const
		{
			return Count != Other.Count || Head != Other.Head || Tail != Other.Tail;
		}

		void Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			Instance->Data = Value;
		}

		void Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			Instance->Data = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			Memory::Construct<T>(&Instance->Data, args...);
		}

		void AppendBack(const T& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Value;

			AppendNodeTail(Instance);
		}

		void AppendBack(T&& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			AppendNodeTail(Instance);
		}

		template<typename... Args>
		void AppendBackConstruct(Args&&... args)
		{
			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			AppendNodeTail(Instance);
		}

		void AppendBackRange(const LinkedList<T>& Other)
		{
			Node* Instance = Other.Head;
			while (Instance != nullptr)
			{
				AppendBack(Instance->Data);
				Instance = Instance->Next;
			}
		}

		void AppendFront(const T& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Value;

			AppendNodeHead(Instance);
		}

		void AppendFront(T&& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			AppendNodeHead(Instance);
		}

		template<typename... Args>
		void AppendFrontConstruct(Args&&... args)
		{
			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			AppendNodeHead(Instance);
		}

		void AppendFrontRange(const LinkedList<T>& Other)
		{
			Node* Instance = Other.Head;
			while (Instance != nullptr)
			{
				AppendFront(Instance->Data);
				Instance = Instance->Next;
			}
		}

		void InsertBack(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = CreateNode();
			Instance->Data = Value;

			Node* Anchor = NodeFromData(Position);
			InsertNodeBack(Anchor, Instance);
		}

		void InsertBack(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			Node* Anchor = NodeFromData(Position);
			InsertNodeBack(Anchor, Instance);
		}

		template<typename... Args>
		void InsertBackConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = NodeFromData(Position);
			InsertNodeBack(Anchor, Instance);
		}

		void InsertBackRange(T* Position, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Other.Head;
			Node* Anchor = NodeFromData(Position);

			while (Instance != nullptr)
			{
				InsertNodeBack(Anchor, Instance->Data);
				Instance = Instance->Next;
				Anchor = Anchor->Next;
			}
		}

		void InsertFront(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = CreateNode();
			Instance->Data = Value;

			Node* Anchor = NodeFromData(Position);
			InsertNodeFront(Anchor, Instance);
		}

		void InsertFront(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			Node* Anchor = NodeFromData(Position);
			InsertNodeFront(Anchor, Instance);
		}

		template<typename... Args>
		void InsertFrontConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = NodeFromData(Position);
			InsertNodeFront(Anchor, Instance);
		}

		void InsertFrontRange(T* Position, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Other.Head;
			Node* Anchor = NodeFromData(Position);

			while (Instance != nullptr)
			{
				InsertNodeFront(Anchor, Instance->Data);
				Instance = Instance->Next;
				Anchor = Anchor->Next;
			}
		}

		void RemoveBack()
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			Node* Instance = RemoveNodeTail();
			DestroyNode(Instance);
		}

		void RemoveBack(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			while (Tail != Instance)
			{
				RemoveBack();
			}
			RemoveBack();
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			Node* Instance = RemoveNodeHead();
			DestroyNode(Instance);
		}

		void RemoveFront(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			while (Head != Instance)
			{
				RemoveFront();
			}
			RemoveFront();
		}

		void Remove(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Start = NodeFromData(From);
			Node* End = NodeFromData(To);
		
			Node* Prev = Start->Prev;
			Node* Next = End->Next;

			while (Start != End)
			{
				Node* Instance = Start;
				Start = Start->Next;
				DestroyNode(Instance);
			}

			DestroyNode(End);

			if (Prev)
			{
				Prev->Next = Next;
			}
			else
			{
				Head = Next;
			}

			if (Next)
			{
				Next->Prev = Prev;
			}
			else
			{
				Tail = Prev;
			}
		}

		void Clear()
		{
			while(Tail)
			{
				RemoveBack();
			}
		}

		T& GetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			NEXUS_ASSERT(Instance != Tail, "Cannot get next on Tail");

			return Instance->Next->Data;
		}

		T* TryGetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			if (Instance == Tail)
			{
				return nullptr;
			}

			return &Instance->Next->Data;
		}

		T& GetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			NEXUS_ASSERT(Instance != Head, "Cannot get prev on Head");

			return Instance->Prev->Data;
		}

		T* TryGetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = NodeFromData(Position);
			if (Instance == Head)
			{
				return nullptr;
			}

			return &Instance->Prev->Data;
		}

		T& First() const
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			return Head->Data;
		}

		T& Last() const
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

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
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
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
			Node* Current = Head;
			while (Current != nullptr)
			{
				if (Current->Data == Other)
				{
					return true;
				}

				Current = Current->Next;
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
			Node* H = Head;
			Node* T = Tail;
			for (uint64 It = 0; It < Half; It++)
			{
				Node* Next = H->Next;
				Node* Prev = T->Prev;
				
				Swap(&H->Data, &T->Data);

				H = Next;
				T = Prev;
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

		void AppendNodeTail(Node* Instance)
		{
			Instance->Next = nullptr;
			Instance->Prev = Tail;

			if (!Head)
			{
				Head = Instance;
			}

			if (Tail)
			{
				Tail->Next = Instance;
			}

			Tail = Instance;
		}

		void AppendNodeHead(Node* Instance)
		{
			Instance->Next = Head;
			Instance->Prev = nullptr;

			if (!Tail)
			{
				Tail = Instance;
			}

			if (Head)
			{
				Head->Prev = Instance;
			}

			Head = Instance;
		}

		void InsertNodeBack(Node* Anchor, Node* Instance)
		{
			Instance->Next = Anchor->Next;
			Instance->Prev = Anchor;
			Anchor->Next->Prev = Instance;
			Anchor->Next = Instance;
		}

		void InsertNodeFront(Node* Anchor, Node* Instance)
		{
			Instance->Prev = Anchor->Prev;
			Instance->Next = Anchor;
			Anchor->Prev->Next= Instance;
			Anchor->Prev = Instance;
		}

		Node* RemoveNodeTail()
		{
			Node* Instance = Tail;
			Tail = Tail->Prev;

			if (Tail)
			{
				Tail->Next = nullptr;
			}
			
			if (Head == Instance)
			{
				Head = nullptr;
			}

			return Instance;
		}

		Node* RemoveNodeHead()
		{
			Node* Instance = Head;
			Head = Head->Next;

			if (Head)
			{
				Head->Prev = nullptr;
			}

			if (Tail == Instance)
			{
				Tail = nullptr;
			}

			return Instance;
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