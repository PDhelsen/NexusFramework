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
			T Value;
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
				Pointer = &(GetNode(Pointer)->Next->Value);
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
				Pointer = &(GetNode(Pointer)->Prev->Value);
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
				Copy.AppendBack(Current->Value);
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

		T& Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			Memory::Construct<T>(&Instance->Value, args...);
			return Instance->Value;
		}

		T& AppendBack(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Value = Value;

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		T& AppendBack(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendBackConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		T& AppendBackRange(const LinkedList<T>& Other)
		{
			Node Return = DataTail;

			Node* Instance = Other.DataHead;
			while (Instance != nullptr)
			{
				AppendBack(Instance->Value);
				Instance = Instance->Next;
			}
			return Return->Next->Value;
		}

		T& AppendFront(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Value = Value;

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		T& AppendFront(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendFrontConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		T& AppendFrontRange(const LinkedList<T>& Other)
		{
			Node* Instance = Other.DataHead;
			while (Instance != nullptr)
			{
				AppendFront(Instance->Value);
				Instance = Instance->Next;
			}

			return DataHead->Value;
		}

		T& InsertBack(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Instance->Value = Value;

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertBack(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertBackConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertBackRange(T* Position, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Return = DataTail;

			Node* Instance = Other.DataHead;
			Node* Anchor = GetNode(Position);

			while (Instance != nullptr)
			{
				InsertNodeBack(Anchor, Instance->Value);
				Instance = Instance->Next;
				Anchor = Anchor->Next;
			}

			return Return->Next->Value;
		}

		T& InsertFront(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Instance->Value = Value;

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertFront(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertFrontConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertFrontRange(T* Position, const LinkedList<T>& Other)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = Other.DataHead;
			Node* Anchor = GetNode(Position);

			while (Instance != nullptr)
			{
				InsertNodeFront(Anchor, Instance->Value);
				Instance = Instance->Next;
				Anchor = Anchor->Next;
			}

			return DataHead->Value;
		}

		void RemoveBack()
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			Node* Instance = DataTail;
			RemoveNode(Instance);
			Free(Instance);
		}

		void RemoveBack(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position)->Prev;
			while (DataTail != Instance)
			{
				RemoveBack();
			}
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			Node* Instance = DataHead;
			RemoveNode(Instance);
			Free(Instance);
		}

		void RemoveFront(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position)->Next;
			while (DataHead != Instance)
			{
				RemoveFront();
			}
		}

		void Remove(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			RemoveNode(Instance);
			Free(Instance);
		}

		void Remove(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Start = GetNode(From);
			Node* End = GetNode(To)->Next;
		
			while (Start != End)
			{
				Node* Instance = Start;
				Start = Start->Next;
				RemoveNode(Instance);
				Free(Instance);
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

			return Instance->Next->Value;
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

			return &Instance->Next->Value;
		}

		T& GetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			Node* Instance = GetNode(Position);
			NEXUS_ASSERT(Instance != DataHead, "Cannot get prev on Head");

			return Instance->Prev->Value;
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

			return &Instance->Prev->Value;
		}

		T& First() const
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			return DataHead->Value;
		}

		T& Last() const
		{
			NEXUS_ASSERT(!IsEmpty(), "List is empty");

			return DataTail->Value;
		}

		bool IsNext(T* Position, T* Next) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			return GetNode(Position)->Next == GetNode(Next);
		}

		bool IsPrevious(T* Position, T* Prev) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			return GetNode(Position)->Prev == GetNode(Prev);
		}

		bool IsConnected(T* A, T* B) const
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");
			NEXUS_ASSERT(!IsEmpty(), "List is empty");
			
			return IsNext(A, B) || IsPrev(A, B);
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&DataHead->Value);
		}

		Iterator BeginReverse() const
		{
			return Iterator(&DataTail->Value);
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

			T Temp = GetNode(A)->Value;
			GetNode(A)->Value = Move(GetNode(B)->Value);
			GetNode(B)->Value = Move(Temp);
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
				if (Current->Value == Other)
				{
					return &Current->Value;
				}

				Current = Current->Next;
			}

			return nullptr;
		}

		void Sort()
		{
			SortSort(&DataHead);
			FixLinks();
		}

		void Reverse()
		{
			DataTail = DataHead;

			Node* Current = DataHead;
			Node* Next = Current->Next;
			Current->Next = nullptr;
			Current->Prev = Next;

			while (Next)
			{
				Node* SecondNext = Next->Next;
				Next->Next = Current;
				Next->Prev = SecondNext;

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
			Instance->Prev = nullptr;
			return Instance;
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

		void RemoveNode(Node* Instance)
		{
			if (Instance == DataHead)
			{
				DataHead = DataHead->Next;

				if (DataHead)
				{
					DataHead->Prev = nullptr;
				}

				if (DataTail == Instance)
				{
					DataTail = nullptr;
				}
			}
			else if (Instance == DataTail)
			{
				DataTail = DataTail->Prev;

				if (DataTail)
				{
					DataTail->Next = nullptr;
				}

				if (DataHead == Instance)
				{
					DataHead = nullptr;
				}
			}
			else
			{
				Node* Next = Instance->Next;
				Node* Prev = Instance->Prev;

				Next->Prev = Prev;
				Prev->Next = Next;
			}
		}

		static Node* GetNode(T* Value)
		{
			return reinterpret_cast<Node*>(Value);
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

			if (A->Value <= B->Value)
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