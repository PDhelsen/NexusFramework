#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Tree
	{
		struct Node
		{
			T Value;
			uint64 Count;
			Node* Parent;
			Node* Sibling;
			Node* Child;
		};

	public:
		class Iterator
		{
		public:
			Iterator(T* Ptr)
			{
				Current = GetNode(Ptr);
			}

			Iterator& operator++()
			{
				if (Current->Child)
				{
					Current = Current->Child;
				}
				else if (Current->Sibling)
				{
					Current = Current->Sibling;
				}
				else
				{
					while (Current)
					{
						if (Current->Parent && Current->Parent->Sibling)
						{
							Current = Current->Parent->Sibling;
							break;
						}

						Current = Current->Parent;
					}
				}

				return *this;
			}

			Iterator operator++(int32)
			{
				Iterator Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return *(Current->Value);
			}

			T& operator*() const
			{
				return Current->Value;
			}

			bool operator==(const Iterator& Other) const
			{
				return Current == Other.Current;
			}

			bool operator!=(const Iterator& Other) const
			{
				return Current != Other.Current;
			}

		private:
			Node* Current;
		};

		Tree(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Tree(const Tree<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
		}

		Tree(Tree<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
			Data = nullptr;
		}

		~Tree()
		{
			Clear();
		}

		Tree<T> Copy() const
		{
			Tree<T> Copy = Tree<T>(Allocator);
			Copy.Initialize(Data->Value);
			
			Node* RootChild = Data->Child;
			while (RootChild)
			{
				Copy.CopyNode(Copy.Data, RootChild);
				RootChild = RootChild->Sibling;
			}
			return Copy;
		}

		bool operator==(const Tree<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Tree<T>& Other) const
		{
			return !(*this == Other);
		}

		T& Initialize(const T& Value)
		{
			Data = Allocate();
			Data->Value = Value;
			return Data->Value;
		}

		T& Initialize(T&& Value)
		{
			Data = Allocate();
			Data->Value = Move(Value);
			return Data->Value;
		}

		template<typename... Args>
		T& InitializeConstruct(Args&&... args)
		{
			Data = Allocate();
			Memory::Construct<T>(&Data->Value, args...);
			return Data->Value;
		}

		T& Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = GetNode(Position);
			Memory::Construct<T>(&Instance->Value, args...);
			return Instance->Value;
		}

		T& Append(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Instance->Value = Value;

			Node* Anchor = GetNode(Parent);
			AppendNode(Anchor, Instance);
			return Instance->Value;
		}

		T& Append(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			Node* Anchor = GetNode(Parent);
			AppendNode(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendConstruct(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			Node* Anchor = GetNode(Parent);
			AppendNode(Anchor, Instance);
			return Instance->Value;
		}

		T& AppendRange(T* Parent, const Tree<T>& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Anchor = GetNode(Parent);
			Node* Copy = GetNode(&Value.GetRoot());
			return CopyNode(Anchor, Copy);
		}

		T& InsertSibling(T* Sibling, const T& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Instance->Value = Value;
		
			Node* Anchor = GetNode(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertSibling(T* Sibling, T&& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			Node* Anchor = GetNode(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertSiblingConstruct(T* Sibling, Args&&... args)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			Node* Anchor = GetNode(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertChild(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Instance->Value = Value;

			Node* Anchor = GetNode(Parent);
			InsertNodeChild(Anchor, Instance);
			return Instance->Value;
		}

		T& InsertChild(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			Node* Anchor = GetNode(Parent);
			InsertNodeChild(Anchor, Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& InsertChildConstruct(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			Node* Anchor = GetNode(Parent);
			InsertNodeChild(Anchor, Instance);
			return Instance->Value;
		}

		void Remove(T* Root)
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			Node* Instance = GetNode(Root);
			RemoveNode(Instance);
		}

		void RemoveChildren(T* Root)
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			Node* Instance = GetNode(Root);
			Node* Child = Instance->Child;
			while (Child)
			{
				Node* Sibling = Child->Sibling;
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

		T& GetRoot() const 
		{
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return Data->Value;
		}

		T* TryGetRoot() const
		{
			if (!Data)
			{
				return nullptr;
			}

			return &Data->Value;
		}

		T& GetParent(T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");
			
			return GetNode(Child)->Parent->Value;
		}

		T* TryGetParent(T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = GetNode(Child);
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
			
			return GetNode(Sibling)->Sibling->Value;
		}

		T* TryGetSibling(T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = GetNode(Sibling);
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
			
			return GetNode(Parent)->Child->Value;
		}

		T* TryGetChild(T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			Node* Instance = GetNode(Parent);
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
			
			return GetNode(Position)->Parent == GetNode(Parent);
		}

		bool IsSibling(T* Position, T* Sibling) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty"); 
			
			return GetNode(Position)->Parent == GetNode(Sibling)->Parent;
		}

		bool IsChild(T* Position, T* Child) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty"); 
			
			return GetNode(Position) == GetNode(Child)->Parent;
		}

		bool IsConnected(T* A, T* B) const
		{
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");
			NEXUS_ASSERT(!IsEmpty(), "Tree is empty");

			return IsParent(A, B) || IsSibling(A, B) || IsChild(A, B);
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&Data->Value);
		}

		Iterator end() const { return End(); }
		Iterator End() const
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
			for (Iterator It = Begin(); It != End(); It++)
			{
				if (*It == Other)
				{
					return &(*It);
				}
			}

			return nullptr;
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		bool IsInitialized() const { Data != nullptr; }
		uint64 GetChildCount(T* Instance) const { return GetNode(Instance)->Count; }
		uint64 GetSiblingCount(T* Instance) const { return GetNode(Instance)->Parent->Count - 1; }

	private:
		Node* Allocate()
		{
			Count++;

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Count = 0;
			Instance->Parent = nullptr;
			Instance->Sibling = nullptr;
			Instance->Child = nullptr;
			return Instance;
		}

		void Free(Node* Instance)
		{
			Count--;

			Memory::Destruct(&Instance->Value);
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* Parent, Node* Child)
		{
			Child->Parent = Parent;

			if (Parent->Child)
			{
				Node* Sibling = Parent->Child;
				while (Sibling->Sibling != nullptr)
				{
					Sibling = Sibling->Sibling;
				}
				Sibling->Sibling = Child;
			}
			else
			{
				Parent->Child = Child;
			}

			Parent->Count++;
		}

		void InsertNodeSibling(Node* Sibling, Node* Instance)
		{
			Instance->Parent = Sibling->Parent;
			Instance->Sibling = Sibling->Sibling;
			Sibling->Sibling = Instance;
			Sibling->Parent->Count++;
		}

		void InsertNodeChild(Node* Parent, Node* Instance)
		{
			Node* Child = Parent->Child;
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

		void RemoveNode(Node* Instance)
		{
			if (!Instance)
			{
				return;
			}

			Node* Child = Instance->Child;
			while (Child)
			{
				Node* Sibling = Child->Sibling;
				RemoveNode(Child);
				Child = Sibling;
			}

			Node* Parent = Instance->Parent;
			if (Parent)
			{
				Node* Sibling = Parent->Child;
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
				Parent->Count--;
			}

			Free(Instance);
		}

		T& CopyNode(Node* Parent, Node* Instance)
		{
			T& Return = Append(&Parent->Value, Instance->Value);

			Node* Copy = Parent->Child;
			while (Copy->Sibling)
			{
				Copy = Copy->Sibling;
			}
			
			Node* Child = Instance->Child;
			while (Child)
			{
				CopyNode(Copy, Child);
				Child = Child->Sibling;
			}

			return Return;
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
		Node* Data;
	};
}