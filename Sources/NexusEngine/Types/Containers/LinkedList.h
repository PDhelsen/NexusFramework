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
		using N = Node::NodeDouble<T>;
		using I = Iterator::IteratorNodeDouble<T, N>;

		LinkedList(Allocator* Allctr = nullptr)
			: Alloc(Allctr), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		LinkedList(const LinkedList<T>& Other)
			: Alloc(Other.Alloc), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "LinkedList - Copy constructor");

			N* Current = Other.DataHead;
			while (Current)
			{
				AppendBack(Current->Value);
				Current = Current->Next;
			}
		}

		LinkedList(LinkedList<T>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
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

			Alloc = Other.Alloc;

			N* Current = Other.DataHead;
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

			Alloc = Other.Alloc;
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

			N* Instance = Node::GetNode<T, N>(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Destruct(Instance);
			Construct(Instance, args...);
			return Instance->Value;
		}

		T& AppendBack(const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		T& AppendBack(T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendBackConstruct(Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			AppendNodeTail(Instance);
			return Instance->Value;
		}

		template<typename C>
		T& AppendBackRange(const C& Value)
		{
			N* Return = DataTail;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				AppendBack(*It);
			}

			return Return->Next->Value;
		}

		T& AppendFront(const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		T& AppendFront(T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendFrontConstruct(Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			AppendNodeHead(Instance);
			return Instance->Value;
		}

		template<typename C>
		T& AppendFrontRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				AppendFront(*It);
			}

			return DataHead->Value;
		}

		T& InsertBack(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = Node::GetNode<T, N>(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertBack(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = Node::GetNode<T, N>(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertBackConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = Node::GetNode<T, N>(Position);
			InsertNodeBack(Anchor, Instance);
			return Instance->Value;
		}

		template<typename C>
		T& InsertBackRange(T* Position, const C& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Anchor = Node::GetNode<T, N>(Position);
			N* Return = Anchor;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
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

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = Node::GetNode<T, N>(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertFront(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = Node::GetNode<T, N>(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertFrontConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = Node::GetNode<T, N>(Position);
			InsertNodeFront(Anchor, Instance);
			return Instance->Value;
		}

		template<typename C>
		T& InsertFrontRange(T* Position, const C& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Anchor = Node::GetNode<T, N>(Position);

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				InsertFront(&Anchor->Value, *It);
				Anchor = Anchor->Prev;
			}

			return Anchor->Value;
		}

		void RemoveBack()
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			N* Instance = DataTail;
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void RemoveBack(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position)->Prev;
			while (DataTail != Instance)
			{
				RemoveBack();
			}
		}

		void RemoveFront()
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			N* Instance = DataHead;
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void RemoveFront(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position)->Next;
			while (DataHead != Instance)
			{
				RemoveFront();
			}
		}

		void Remove(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void Remove(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Start = Node::GetNode<T, N>(From);
			N* End = Node::GetNode<T, N>(To)->Next;
		
			while (Start != End)
			{
				N* Instance = Start;
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

			N* Instance = Node::GetNode<T, N>(Position);
			NEXUS_ASSERT(Instance != DataTail, "Cannot get next on Tail");

			return Instance->Next->Value;
		}

		T* TryGetNext(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position);
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

			N* Instance = Node::GetNode<T, N>(Position);
			NEXUS_ASSERT(Instance != DataHead, "Cannot get prev on Head");

			return Instance->Prev->Value;
		}

		T* TryGetPrev(T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Node::GetNode<T, N>(Position);
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
			
			return Node::GetNode<T, N>(Position)->Next == Node::GetNode<T, N>(Next);
		}

		bool IsPrevious(T* Position, T* Prev) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return Node::GetNode<T, N>(Position)->Prev == Node::GetNode<T, N>(Prev);
		}

		bool IsConnected(T* A, T* B) const
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return IsNext(A, B) || IsPrev(A, B);
		}

		I GetIterator(T* Position)
		{
			return I(Node::GetNode<T, N>(Position));
		}

		I begin() const { return Begin(); }
		I Begin() const
		{
			return I(DataHead);
		}

		I BeginReverse() const
		{
			return I(DataTail);
		}

		I end() const { return End(); }
		I End() const
		{
			return I(nullptr);
		}

		I EndReverse() const
		{
			return I(nullptr);
		}

		void Swap(T* A, T* B)
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			T Temp = Node::GetNode<T, N>(A)->Value;
			Node::GetNode<T, N>(A)->Value = Move(Node::GetNode<T, N>(B)->Value);
			Node::GetNode<T, N>(B)->Value = Move(Temp);
		}

		void Reverse()
		{
			DataTail = DataHead;

			N* Current = DataHead;
			N* Next = Current->Next;
			Current->Next = nullptr;
			Current->Prev = Next;

			while (Next)
			{
				N* SecondNext = Next->Next;
				Next->Next = Current;
				Next->Prev = SecondNext;

				Current = Next;
				Next = SecondNext;
			}

			DataHead = Current;
		}

		template<typename S = Sorting::DefaultLinkBased>
		void Sort(Sorting::CompareFunction<T> Function = nullptr)
		{
			Sort::SortLinkBased<T, S, N>(&DataHead, Function);
			Node::FixupNode<N>(DataHead, &DataTail);
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		I Find(const T& Other) const
		{
			for (I It = Begin(); It != End(); ++It)
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
		N* Allocate()
		{
			++Count;

			N* Instance = (N*)Memory::Allocate(sizeof(N), NEXUS_MEMORY_ALIGN, Alloc);
			Instance->Next = nullptr;
			Instance->Prev = nullptr;
			return Instance;
		}

		void Free(N* Instance)
		{
			--Count;

			Memory::Free(Instance, Alloc);
		}

		template<typename... Args>
		void Construct(N* Instance, Args&&... args)
		{
			Memory::Construct<T>(&Instance->Value, args...);
		}

		void Destruct(N* Instance)
		{
			Memory::Destruct(&Instance->Value);
		}

		void AppendNodeTail(N* Instance)
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

		void AppendNodeHead(N* Instance)
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

		void InsertNodeBack(N* Anchor, N* Instance)
		{
			Instance->Next = Anchor->Next;
			Instance->Prev = Anchor;
			if (Anchor->Next)
			{
				Anchor->Next->Prev = Instance;
			}
			Anchor->Next = Instance;
		}

		void InsertNodeFront(N* Anchor, N* Instance)
		{
			Instance->Prev = Anchor->Prev;
			Instance->Next = Anchor;
			if (Anchor->Prev)
			{
				Anchor->Prev->Next= Instance;
			}
			Anchor->Prev = Instance;
		}

		void RemoveNode(N* Instance)
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
				N* Next = Instance->Next;
				N* Prev = Instance->Prev;

				Next->Prev = Prev;
				Prev->Next = Next;
			}
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Alloc;
		uint64 Count;
		N* DataHead;
		N* DataTail;
	};
}
