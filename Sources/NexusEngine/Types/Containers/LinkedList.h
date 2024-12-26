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
		}

		LinkedList(const LinkedList<T>& Other)
			: Alloc(Other.Alloc), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			NEXUS_LOG(Warning, LoggerChannel::Performance, "LinkedList - Copy constructor");

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
			Other.Count = 0;
			Other.DataHead = nullptr;
			Other.DataTail = nullptr;
		}

		~LinkedList()
		{
			Clear();
		}

		LinkedList<T>& operator=(const LinkedList<T>& Other)
		{
			NEXUS_LOG(Warning, LoggerChannel::Performance, "LinkedList - Assignement operator");

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

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Item = Value;
			return Item;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Item = Move(Value);
			return Item;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Destruct(Instance);
			Construct(Instance, args...);
			return Item;
		}

		T& AppendBack(const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			AppendNodeTail(Instance);
			return GetItem(Instance);
		}

		T& AppendBack(T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNodeTail(Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& AppendBackConstruct(Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			AppendNodeTail(Instance);
			return GetItem(Instance);
		}

		template<typename C>
		T& AppendBackRange(const C& Value)
		{
			N* Return = DataTail;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				AppendBack(*It);
			}

			return GetItem(Return->Next);
		}

		T& AppendFront(const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			AppendNodeHead(Instance);
			return GetItem(Instance);
		}

		T& AppendFront(T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNodeHead(Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& AppendFrontConstruct(Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			AppendNodeHead(Instance);
			return GetItem(Instance);
		}

		template<typename C>
		T& AppendFrontRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				AppendFront(*It);
			}

			return GetItem(DataHead);
		}

		T& InsertBack(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return GetItem(Instance);
		}

		T& InsertBack(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& InsertBackConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = GetNode(Position);
			InsertNodeBack(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename C>
		T& InsertBackRange(T* Position, const C& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Anchor = GetNode(Position);
			N* Return = Anchor;

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				InsertBack(&Anchor->Value, *It);
				Anchor = Anchor->Next;
			}

			return GetItem(Return->Next);
		}

		T& InsertFront(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return GetItem(Instance);
		}

		T& InsertFront(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& InsertFrontConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = GetNode(Position);
			InsertNodeFront(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename C>
		T& InsertFrontRange(T* Position, const C& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Anchor = GetNode(Position);

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				InsertFront(&Anchor->Value, *It);
				Anchor = Anchor->Prev;
			}

			return GetItem(Anchor);
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

			N* Instance = GetNode(Position)->Prev;
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

			N* Instance = GetNode(Position)->Next;
			while (DataHead != Instance)
			{
				RemoveFront();
			}
		}

		void Remove(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void Remove(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Start = GetNode(From);
			N* End = GetNode(To)->Next;
		
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

		T& Get() 
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return GetItem(DataHead);
		}

		const T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return GetItem(DataHead);
		}

		T& GetNext(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(GetNode(Position) != DataTail, "Cannot get next on Tail");

			N* Instance = GetNode(Position);
			return GetItem(Instance->Next);
		}

		const T& GetNext(const T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(GetNode(Position) != DataTail, "Cannot get next on Tail");

			N* Instance = GetNode(Position);
			return GetItem(Instance->Next);
		}

		T* TryGetNext(T* Position) 
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			if (Instance == DataTail)
			{
				return nullptr;
			}
			return &GetItem(Instance->Next);;
		}

		const T* TryGetNext(const T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			if (Instance == DataTail)
			{
				return nullptr;
			}
			return &GetItem(Instance->Next);
		}

		T& GetPrev(T* Position) 
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(GetNode(Position) != DataHead, "Cannot get prev on Head");

			N* Instance = GetNode(Position);
			return GetItem(Instance->Prev);
		}

		const T& GetPrev(const T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(GetNode(Position) != DataHead, "Cannot get prev on Head");

			N* Instance = GetNode(Position);
			return GetItem(Instance->Prev);
		}

		T* TryGetPrev(T* Position) 
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			if (Instance == DataHead)
			{
				return nullptr;
			}
			return &GetItem(Instance->Prev);
		}

		const T* TryGetPrev(const T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			N* Instance = GetNode(Position);
			if (Instance == DataHead)
			{
				return nullptr;
			}
			return &GetItem(Instance->Prev);
		}

		T& First() 
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return GetItem(DataHead);
		}

		const T& First() const
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return GetItem(DataHead);
		}

		T& Last() 
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return GetItem(DataTail);
		}

		const T& Last() const
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");

			return GetItem(DataTail);
		}

		bool IsNext(const T* Position, const T* Next) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return GetNode(Position)->Next == GetNode(Next);
		}

		bool IsPrevious(const T* Position, const T* Prev) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return GetNode(Position)->Prev == GetNode(Prev);
		}

		bool IsConnected(const T* A, const T* B) const
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			
			return IsNext(A, B) || IsPrev(A, B);
		}

		I GetIterator(T* Position)
		{
			return GetIteratorNode(GetNode(Position));
		}

		const I GetIterator(const T* Position) const
		{
			return GetIteratorNode(GetNode(Position));
		}

		I begin() { return Begin(); }
		I Begin() 
		{
			return GetIteratorNode(DataHead);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIteratorNode(DataHead);
		}

		I BeginReverse() 
		{
			return GetIteratorNode(DataTail);
		}

		const I BeginReverse() const
		{
			return GetIteratorNode(DataTail);
		}

		I end() { return End(); }
		I End()
		{
			return I(nullptr);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return I(nullptr);
		}

		I EndReverse() 
		{
			return I(nullptr);
		}

		const I EndReverse() const
		{
			return I(nullptr);
		}

		void Swap(T* A, T* B)
		{
			NEXUS_ASSERT(!IsEmpty(), "LinkedList is empty");
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			T Temp = GetItem(GetNode(A));
			GetItem(GetNode(A)) = Move(GetItem(GetNode(B)));
			GetItem(GetNode(B)) = Move(Temp);
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
			return GetIteratorValue(Other) != End();
		}

		I Find(const T& Other) 
		{
			return GetIteratorValue(Other);
		}

		const I Find(const T& Other) const
		{
			return GetIteratorValue(Other);
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }

	private:
		N* Allocate()
		{
			++Count;

			N* Instance = (N*)Memory::Allocate(sizeof(N), Alloc);
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

		N* GetNode(T* Position)
		{
			return Node::GetNode<T, N>(Position);
		}

		const N* GetNode(const T* Position) const
		{
			return Node::GetNode<T, N>(Position);
		}

		T& GetItem(N* Instance)
		{
			return Instance->Value;
		}

		const T& GetItem(const N* Instance) const
		{
			return Instance->Value;
		}

		I GetIteratorNode(N* Instance)
		{
			return I(Instance);
		}

		const I GetIteratorNode(const N* Instance) const
		{
			return I(const_cast<N*>(Instance));
		}

		I GetIteratorValue(const T& Value) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (*It == Value)
				{
					return It;
				}
			}

			return End();
		}

		Allocator* Alloc;
		uint64 Count;
		N* DataHead;
		N* DataTail;
	};
}
