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
				Pointer = &(GetNode(Pointer)->Next->Data);
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
				Pointer = &(GetNode(Pointer)->Prev->Data);
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
			: Allocator(Allctr), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		LinkedList(const LinkedList<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
		{
		}

		LinkedList(LinkedList<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
		{
			Other.DataHead = nullptr;
			Other.DataTail = nullptr;
		}

		~LinkedList()
		{
			Clear();
		}

		LinkedList<T> Copy() const
		{
			LinkedList Copy = LinkedList(Allocator);

			Node* Current = DataHead;
			while (Current != nullptr)
			{
				Copy.AppendBack(Current->Data);
				Current = Current->Next;
			}

			return Copy;
		}

		bool operator==(const LinkedList<T>& Other) const
		{
			return Count == Other.Count && DataHead == Other.DataHead && DataTail == Other.DataTail;
		}

		bool operator!=(const LinkedList<T>& Other) const
		{
			return !(*this == Other);
		}

		void Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			Instance->Data = Value;
		}

		void Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			Instance->Data = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			Memory::Construct<T>(&Instance->Data, args...);
		}

		void AppendBack(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Value;

			AppendNodeTail(Instance);
		}

		void AppendBack(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Move(Value);

			AppendNodeTail(Instance);
		}

		template<typename... Args>
		void AppendBackConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Data, args...);

			AppendNodeTail(Instance);
		}

		void AppendBackRange(const LinkedList<T>& Other)
		{
			Node* Instance = Other.DataHead;
			while (Instance != nullptr)
			{
				AppendBack(Instance->Data);
				Instance = Instance->Next;
			}
		}

		void AppendFront(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Value;

			AppendNodeHead(Instance);
		}

		void AppendFront(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Move(Value);

			AppendNodeHead(Instance);
		}

		template<typename... Args>
		void AppendFrontConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Data, args...);

			AppendNodeHead(Instance);
		}

		void AppendFrontRange(const LinkedList<T>& Other)
		{
			Node* Instance = Other.DataHead;
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

			Node* Instance = Allocate();
			Instance->Data = Value;

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
		}

		void InsertBack(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Instance->Data = Move(Value);

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
		}

		template<typename... Args>
		void InsertBackConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
		}

		void InsertBackRange(T* Position, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Other.DataHead;
			Node* Anchor = GetNode(Position);

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

			Node* Instance = Allocate();
			Instance->Data = Value;

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
		}

		void InsertFront(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Instance->Data = Move(Value);

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
		}

		template<typename... Args>
		void InsertFrontConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
		}

		void InsertFrontRange(T* Position, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Other.DataHead;
			Node* Anchor = GetNode(Position);

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
			Free(Instance);
		}

		void RemoveBack(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			while (DataTail != Instance)
			{
				RemoveBack();
			}
			RemoveBack();
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			Node* Instance = RemoveNodeHead();
			Free(Instance);
		}

		void RemoveFront(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			while (DataHead != Instance)
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

			Node* Start = GetNode(From);
			Node* End = GetNode(To);
		
			Node* Prev = Start->Prev;
			Node* Next = End->Next;

			while (Start != End)
			{
				Node* Instance = Start;
				Start = Start->Next;
				Free(Instance);
			}

			Free(End);

			if (Prev)
			{
				Prev->Next = Next;
			}
			else
			{
				DataHead = Next;
			}

			if (Next)
			{
				Next->Prev = Prev;
			}
			else
			{
				DataTail = Prev;
			}
		}

		void Clear()
		{
			while(DataTail)
			{
				RemoveBack();
			}
		}

		T& GetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			NEXUS_ASSERT(Instance != DataTail, "Cannot get next on Tail");

			return Instance->Next->Data;
		}

		T* TryGetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			if (Instance == DataTail)
			{
				return nullptr;
			}

			return &Instance->Next->Data;
		}

		T& GetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			NEXUS_ASSERT(Instance != DataHead, "Cannot get prev on Head");

			return Instance->Prev->Data;
		}

		T* TryGetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			if (Instance == DataHead)
			{
				return nullptr;
			}

			return &Instance->Prev->Data;
		}

		T& First() const
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			return DataHead->Data;
		}

		T& Last() const
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			return DataTail->Data;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&DataHead->Data);
		}

		Iterator BeginReverse() const
		{
			return Iterator(&DataTail->Data);
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

			Node* NodeA = GetNode(A);
			Node* NodeB = GetNode(B);

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

			if (NodeA == DataHead)
			{
				DataHead = NodeB;
			}
			else if (NodeB == DataHead)
			{
				DataHead = NodeA;
			}
			if (NodeA == DataTail)
			{
				DataTail = NodeB;
			}
			else if (NodeB == DataTail)
			{
				DataTail = NodeA;
			}
		}

		bool Contains(const T& Other) const
		{
			Node* Current = DataHead;
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
			SortSort(&DataHead);
			FixLinks();
		}

		void Reverse()
		{
			uint64 Half = Count / 2;
			Node* H = DataHead;
			Node* T = DataTail;
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
		Node* Allocate()
		{
			Count++;
			return (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
		}

		void Free(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AppendNodeTail(Node* Instance)
		{
			Instance->Next = nullptr;
			Instance->Prev = DataTail;

			if (!DataHead)
			{
				DataHead = Instance;
			}

			if (DataTail)
			{
				DataTail->Next = Instance;
			}

			DataTail = Instance;
		}

		void AppendNodeHead(Node* Instance)
		{
			Instance->Next = DataHead;
			Instance->Prev = nullptr;

			if (!DataTail)
			{
				DataTail = Instance;
			}

			if (DataHead)
			{
				DataHead->Prev = Instance;
			}

			DataHead = Instance;
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
			Node* Instance = DataTail;
			DataTail = DataTail->Prev;

			if (DataTail)
			{
				DataTail->Next = nullptr;
			}
			
			if (DataHead == Instance)
			{
				DataHead = nullptr;
			}

			return Instance;
		}

		Node* RemoveNodeHead()
		{
			Node* Instance = DataHead;
			DataHead = DataHead->Next;

			if (DataHead)
			{
				DataHead->Prev = nullptr;
			}

			if (DataTail == Instance)
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
			Node* Current = DataHead;
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
					DataTail = Prev;
				}
			}
		}

		Allocator* Allocator;
		uint64 Count;
		Node* DataHead;
		Node* DataTail;
	};
}