#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class LinkedList
	{
	public:
		using Node = LinkedNodeDouble<T>;
		using Iterator = LinkedIteratorDouble<T, Node>;

		LinkedList(Allocator* Allctr = nullptr)
			: Allocator(Allctr), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		LinkedList(const LinkedList<T>& Other)
			: Allocator(Other.Allocator), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "LinkedList - Copy constructor");

			Node* Current = Other.DataHead;
			while (Current)
			{
				AppendBack(Current->Value);
				Current = Current->Next;
			}
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

		LinkedList<T>& operator=(const LinkedList<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "LinkedList - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Allocator = Other.Allocator;

			Node* Current = Other.DataHead;
			while (Current)
			{
				AppendBack(Current->Value);
				Current = Current->Next;
			}

			return *this;
		}

		LinkedList<T>& operator=(LinkedList<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Allocator = Other.Allocator;
			Count = Other.Count;
			DataHead = Other.DataHead;
			DataTail = Other.DataTail;

			Other.Count = 0;
			Other.DataHead = nullptr;
			Other.DataTail = nullptr;

			return *this;
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
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			Destruct(Instance);
			Construct(Instance, args...);
			return Instance->Value;
		}

		T& AppendBack(const T& Value)
		{
			Node* Instance = Allocate();
			Construct(Instance, Value);

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		T& AppendBack(T&& Value)
		{
			Node* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendBackConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Construct(Instance, args...);

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		template<typename C>
		T& AppendBackRange(const C& Value)
		{
			Node* Return = DataTail;

			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				AppendBack(*It);
			}

			return Return->Next->Value;
		}

		T& AppendFront(const T& Value)
		{
			Node* Instance = Allocate();
			Construct(Instance, Value);

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		T& AppendFront(T&& Value)
		{
			Node* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendFrontConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Construct(Instance, args...);

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		template<typename C>
		T& AppendFrontRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				AppendFront(*It);
			}

			return DataHead->Value;
		}

		T& InsertBack(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = Allocate();
			Construct(Instance, Value);

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertBack(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = Allocate();
			Construct(Instance, Move(Value));

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertBackConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = Allocate();
			Construct(Instance, args...);

			Node* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		template<typename C>
		T& InsertBackRange(T* Position, const C& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Anchor = GetNode(Position);
			Node* Return = Anchor;

			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				InsertBack(&Anchor->Value, *It);
				Anchor = Anchor->Next;
			}

			return Return->Next->Value;
		}

		T& InsertFront(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = Allocate();
			Construct(Instance, Value);

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertFront(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = Allocate();
			Construct(Instance, Move(Value));

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertFrontConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = Allocate();
			Construct(Instance, args...);

			Node* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		template<typename C>
		T& InsertFrontRange(T* Position, const C& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Anchor = GetNode(Position);

			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				InsertFront(&Anchor->Value, *It);
				Anchor = Anchor->Prev;
			}

			return Anchor->Value;
		}

		void RemoveBack()
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			Node* Instance = DataTail;
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void RemoveBack(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position)->Prev;
			while (DataTail != Instance)
			{
				RemoveBack();
			}
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			Node* Instance = DataHead;
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void RemoveFront(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position)->Next;
			while (DataHead != Instance)
			{
				RemoveFront();
			}
		}

		void Remove(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void Remove(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Start = GetNode(From);
			Node* End = GetNode(To)->Next;
		
			while (Start != End)
			{
				Node* Instance = Start;
				Start = Start->Next;
				RemoveNode(Instance);
				Destruct(Instance);
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

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return DataHead->Value;
		}

		T& GetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			NEXUS_ASSERT(Instance != DataTail, "Cannot get next on Tail");

			return Instance->Next->Value;
		}

		T* TryGetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

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
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			NEXUS_ASSERT(Instance != DataHead, "Cannot get prev on Head");

			return Instance->Prev->Value;
		}

		T* TryGetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			Node* Instance = GetNode(Position);
			if (Instance == DataHead)
			{
				return nullptr;
			}

			return &Instance->Prev->Value;
		}

		T& First() const
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return DataHead->Value;
		}

		T& Last() const
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return DataTail->Value;
		}

		bool IsNext(T* Position, T* Next) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return GetNode(Position)->Next == GetNode(Next);
		}

		bool IsPrevious(T* Position, T* Prev) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return GetNode(Position)->Prev == GetNode(Prev);
		}

		bool IsConnected(T* A, T* B) const
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return IsNext(A, B) || IsPrev(A, B);
		}

		Iterator GetIterator(T* Position)
		{
			return Iterator(GetNode(Position));
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(DataHead);
		}

		Iterator BeginReverse() const
		{
			return Iterator(DataTail);
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
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			T Temp = GetNode(A)->Value;
			GetNode(A)->Value = Move(GetNode(B)->Value);
			GetNode(B)->Value = Move(Temp);
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

		void Sort(Sort::CompareFunction<T> Function = nullptr)
		{
			DataHead = Sort::LinkSort<T>(DataHead, Function);
			
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

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		Iterator Find(const T& Other) const
		{
			for (Iterator It = Begin(); It != End(); It++)
			{
				if (*It == Other)
				{
					return It;
				}
			}

			return End();
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }

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

		template<typename... Args>
		void Construct(Node* Instance, Args&&... args)
		{
			Memory::Construct<T>(&Instance->Value, args...);
		}

		void Destruct(Node* Instance)
		{
			Memory::Destruct(&Instance->Value);
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
			if (Anchor->Next)
			{
				Anchor->Next->Prev = Instance;
			}
			Anchor->Next = Instance;
		}

		void InsertNodeFront(Node* Anchor, Node* Instance)
		{
			Instance->Prev = Anchor->Prev;
			Instance->Next = Anchor;
			if (Anchor->Prev)
			{
				Anchor->Prev->Next= Instance;
			}
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

		Allocator* Allocator;
		uint64 Count;
		Node* DataHead;
		Node* DataTail;
	};
}
