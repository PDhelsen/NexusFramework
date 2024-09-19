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
	class Tree
	{
	public:
		using N = Node::NodeTree<T>;
		using I = Iterator::IteratorNodeTree<T, N>;

		Tree(Allocator* Allctr = nullptr)
			: Alloc(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Tree(const Tree<T>& Other)
			: Alloc(Other.Alloc), Count(0), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Tree - Copy constructor");

			AppendRange(nullptr, Other);
		}

		Tree(Tree<T>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Tree()
		{
			Clear();
		}

		Tree<T>& operator=(const Tree<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Tree - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;

			AppendRange(nullptr, Other);

			return *this;
		}

		Tree<T>& operator=(Tree<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;
			Count = Other.Count;
			Data = Other.Data;

			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
		}

		bool operator==(const Tree<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Tree<T>& Other) const
		{
			return !(*this == Other);
		}

		T& Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Destruct(Instance);
			Construct(Instance, args...);
			return Instance->Value;
		}

		T& Append(T* Parent, const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = Node::GetNode<T, N>(Parent);
			AppendNode(Anchor, Instance);
			return Instance->Value;
		}

		T& Append(T* Parent, T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = Node::GetNode<T, N>(Parent);
			AppendNode(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendConstruct(T* Parent, Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = Node::GetNode<T, N>(Parent);
			AppendNode(Anchor, Instance);
			return Instance->Value;
		}

		T& AppendRange(T* Parent, const Tree<T>& Value)
		{
			N* Anchor = Node::GetNode<T, N>(Parent);
			N* Copy = Node::GetNode<T, N>(&Value.Get());
			return CopyNode(Anchor, Copy);
		}

		template<typename C>
		T& AppendRange(T* Parent, const C& Value)
		{
			typename C::I It = Value.Begin();
			T& Return = Append(Parent, *It);

			for (++It; It != Value.End(); ++It)
			{
				Append(Parent, *It);
			}

			return Return;
		}

		T& InsertSibling(T* Sibling, const T& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = Node::GetNode<T, N>(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertSibling(T* Sibling, T&& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = Node::GetNode<T, N>(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertSiblingConstruct(T* Sibling, Args&&... args)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = Node::GetNode<T, N>(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertChild(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = Node::GetNode<T, N>(Parent);
			InsertNodeChild(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertChild(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = Node::GetNode<T, N>(Parent);
			InsertNodeChild(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertChildConstruct(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = Node::GetNode<T, N>(Parent);
			InsertNodeChild(Anchor, Instance);
			return Instance->Value;
		}

		void Remove(T* Root)
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			N* Instance = Node::GetNode<T, N>(Root);
			RemoveNode(Instance);
		}

		void RemoveChildren(T* Root)
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			N* Instance = Node::GetNode<T, N>(Root);
			N* Child = Instance->Child;
			while (Child)
			{
				N* Sibling = Child->Sibling;
				RemoveNode(Child);
				Child = Sibling;
			}
		}

		void Clear()
		{
			if (IsEmpty())
			{
				return;
			}

			Remove(&Data->Value);
			Data = nullptr;
		}

		T& Get() const 
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return Data->Value;
		}

		T& GetParent(T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return Node::GetNode<T, N>(Child)->Parent->Value;
		}

		T* TryGetParent(T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Node::GetNode<T, N>(Child);
			if (!Instance->Parent)
			{
				return nullptr;
			}
			return &Instance->Parent->Value;
		}

		T& GetSibling(T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return Node::GetNode<T, N>(Sibling)->Sibling->Value;
		}

		T* TryGetSibling(T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Node::GetNode<T, N>(Sibling);
			if (!Instance->Sibling)
			{
				return nullptr;
			}
			return &Instance->Sibling->Value;
		}

		T& GetChild(T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return Node::GetNode<T, N>(Parent)->Child->Value;
		}

		T* TryGetChild(T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			N* Instance = Node::GetNode<T, N>(Parent);
			if (!Instance->Child)
			{
				return nullptr;
			}
			return &Instance->Child->Value;
		}

		bool IsParent(T* Position, T* Parent) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return Node::GetNode<T, N>(Position)->Parent == Node::GetNode<T, N>(Parent);
		}

		bool IsSibling(T* Position, T* Sibling) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty"); 
			
			return Node::GetNode<T, N>(Position)->Parent == Node::GetNode<T, N>(Sibling)->Parent;
		}

		bool IsChild(T* Position, T* Child) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty"); 
			
			return Node::GetNode<T, N>(Position) == Node::GetNode<T, N>(Child)->Parent;
		}

		bool IsConnected(T* A, T* B) const
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			return IsParent(A, B) || IsSibling(A, B) || IsChild(A, B);
		}

		I GetIterator(T* Position)
		{
			return I(Node::GetNode<T, N>(Position));
		}

		I begin() const { return Begin(); }
		I Begin() const
		{
			return I(Data);
		}

		I end() const { return End(); }
		I End() const
		{
			return I(nullptr);
		}

		void Swap(T* A, T* B)
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			T Temp = Node::GetNode<T, N>(A)->Value;
			Node::GetNode<T, N>(A)->Value = Move(Node::GetNode<T, N>(B)->Value);
			Node::GetNode<T, N>(B)->Value = Move(Temp);
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
		bool IsInitialized() const { Data != nullptr; }
		uint64 GetChildCount(T* Instance) const { return Node::GetNode<T, N>(Instance)->Count; }
		uint64 GetSiblingCount(T* Instance) const { return Node::GetNode<T, N>(Instance)->Parent->Count - 1; }

	private:
		N* Allocate()
		{
			++Count;

			N* Instance = (N*)Memory::Allocate(sizeof(N), Alloc);
			Instance->Count = 0;
			Instance->Parent = nullptr;
			Instance->Sibling = nullptr;
			Instance->Child = nullptr;
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

		void AppendNode(N* Parent, N* Child)
		{
			Child->Parent = Parent;

			if (Parent == nullptr && Data == nullptr)
			{
				Data = Child;
			}
			else if (Parent == nullptr && Data != nullptr)
			{
				NEXUS_ASSERT(false, "Trying to replace root of the tree is not allowed");
			}
			else if (Parent->Child)
			{
				N* Sibling = Parent->Child;
				while (Sibling->Sibling != nullptr)
				{
					Sibling = Sibling->Sibling;
				}
				Sibling->Sibling = Child;
				++Parent->Count;
			}
			else
			{
				Parent->Child = Child;
				++Parent->Count;
			}
		}

		void InsertNodeSibling(N* Sibling, N* Instance)
		{
			Instance->Parent = Sibling->Parent;
			Instance->Sibling = Sibling->Sibling;
			Sibling->Sibling = Instance;
			if (Sibling->Parent)
			{
				++Sibling->Parent->Count;
			}
		}

		void InsertNodeChild(N* Parent, N* Instance)
		{
			N* Child = Parent->Child;
			while (Child)
			{
				Child->Parent = Instance;
				Child = Child->Sibling;
			}

			Instance->Parent = Parent;
			Instance->Child = Parent->Child;
			Instance->Count = Parent->Count;
			Parent->Child = Instance;
			Parent->Count = 1;
		}

		void RemoveNode(N* Instance)
		{
			if (!Instance)
			{
				return;
			}

			N* Child = Instance->Child;
			while (Child)
			{
				N* Sibling = Child->Sibling;
				RemoveNode(Child);
				Child = Sibling;
			}

			N* Parent = Instance->Parent;
			if (Parent)
			{
				N* Sibling = Parent->Child;
				if (Sibling == Instance)
				{
					Parent->Child = Instance->Sibling;
				}
				else
				{
					while (Sibling->Sibling != Instance)
					{
						Sibling = Sibling->Sibling;
					}

					Sibling->Sibling = Instance->Sibling;
				}
				--Parent->Count;
			}

			Destruct(Instance);
			Free(Instance);
		}

		T& CopyNode(N* Parent, N* Instance)
		{
			T& Return = Append(Parent != nullptr ? &Parent->Value : nullptr, Instance->Value);

			N* Copy = Node::GetNode<T, N>(&Return);
			N* Child = Instance->Child;
			while (Child)
			{
				CopyNode(Copy, Child);
				Child = Child->Sibling;
			}

			return Return;
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Alloc;
		uint64 Count;
		N* Data;
	};
}
