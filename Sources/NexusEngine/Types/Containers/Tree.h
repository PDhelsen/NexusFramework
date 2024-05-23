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
			T Data;
			uint64 ChildCount;
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
				Current = NodeFromData(Ptr);
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

			T* operator->()
			{
				return *(Current->Data);
			}

			T& operator*()
			{
				return Current->Data;
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
			: Allocator(nullptr), Count(0), Root(nullptr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Tree(const Tree<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
		}

		Tree(Tree<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
			Root = nullptr;
		}

		~Tree()
		{
			Clear();
		}

		Tree<T> Copy() const
		{
			Tree<T> Copy = Tree<T>(Allocator);
			Copy.Initialize(Root->Data);
			
			Node* RootChild = Root->Child;
			while (RootChild)
			{
				Copy.CopyNode(Copy.Root, RootChild);
				RootChild = RootChild->Sibling;
			}
			return Copy;
		}

		bool operator==(const Tree<T>& Other)
		{
			return Count == Other.Count && Root == Other.Root;
		}

		bool operator!=(const Tree<T>& Other)
		{
			return Count != Other.Count || Root != Other.Root;
		}

		void Initialize(const T& Value)
		{
			Root = CreateNode();
			Root->Data = Value;
		}

		void Initialize(T&& Value)
		{
			Root = CreateNode();
			Root->Data = Move(Value);
		}

		template<typename... Args>
		void InitializeConstruct(Args&&... args)
		{
			Root = CreateNode();
			Memory::Construct<T>(&Root->Data, args...);
		}

		void Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");

			Node* Instance = NodeFromData(Position);
			Instance->Data = Value;
		}

		void Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");

			Node* Instance = NodeFromData(Position);
			Instance->Data = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");

			Node* Instance = NodeFromData(Position);
			Memory::Construct<T>(&Instance->Data, args...);
		}

		void Append(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			
			Node* Instance = CreateNode();
			Instance->Data = Value;

			Node* Anchor = NodeFromData(Parent);
			AddChild(Anchor, Instance);
		}

		void Append(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			Node* Anchor = NodeFromData(Parent);
			AddChild(Anchor, Instance);
		}

		template<typename... Args>
		void AppendConstruct(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = NodeFromData(Parent);
			AddChild(Anchor, Instance);
		}

		void AppendRange(T* Parent, const Tree<T>& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Anchor = NodeFromData(Parent);
			Node* Copy = NodeFromData(&Value.GetRoot());
			CopyNode(Anchor, Copy);
		}

		void InsertSibling(T* Sibling, const T& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			
			Node* Instance = CreateNode();
			Instance->Data = Value;
		
			Node* Anchor = NodeFromData(Sibling);
			InsertSibling(Anchor, Instance);
		}

		void InsertSibling(T* Sibling, T&& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			
			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			Node* Anchor = NodeFromData(Sibling);
			InsertSibling(Anchor, Instance);
		}

		template<typename... Args>
		void InsertSiblingConstruct(T* Sibling, Args&&... args)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");

			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = NodeFromData(Sibling);
			InsertSibling(Anchor, Instance);
		}

		void InsertChild(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Instance = CreateNode();
			Instance->Data = Value;

			Node* Anchor = NodeFromData(Parent);
			InsertChild(Anchor, Instance);
		}

		void InsertChild(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Instance = CreateNode();
			Instance->Data = Move(Value);

			Node* Anchor = NodeFromData(Parent);
			InsertChild(Anchor, Instance);
		}

		template<typename... Args>
		void InsertChildConstruct(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);

			Node* Anchor = NodeFromData(Parent);
			InsertChild(Anchor, Instance);
		}

		void Remove(T* Root)
		{
			Node* Instance = NodeFromData(Root);
			RemoveNode(Instance);
		}

		void RemoveChildren(T* Root)
		{
			Node* Instance = NodeFromData(Root);
			Node* Child = Instance->Child;
			while (Child)
			{
				RemoveNode(Child);
				Child = Child->Sibling;
			}
		}

		void Clear()
		{
			Remove(&Root->Data);
			Root = nullptr;
		}

		T& GetRoot() const 
		{
			return Root->Data;
		}

		T* TryGetRoot() const
		{
			if (!Root)
			{
				return nullptr;
			}

			return &Root->Data;
		}

		T& GetParent(T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			return NodeFromData(Child)->Parent->Data;
		}

		T* TryGetParent(T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, "Child is null");
			Node* Instance = NodeFromData(Child);
			if (!Instance->Parent)
			{
				return nullptr;
			}
			return &Instance->Parent->Data;
		}

		T& GetSibling(T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			return NodeFromData(Sibling)->Sibling->Data;
		}

		T* TryGetSibling(T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			Node* Instance = NodeFromData(Sibling);
			if (!Instance->Sibling)
			{
				return nullptr;
			}
			return &Instance->Sibling->Data;
		}

		T& GetChild(T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			return NodeFromData(Parent)->Child->Data;
		}

		T* TryGetChild(T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			Node* Instance = NodeFromData(Parent);
			if (!Instance->Child)
			{
				return nullptr;
			}
			return &Instance->Child->Data;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&Root->Data);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(nullptr);
		}

		void Swap(T* A, T* B)
		{
			Node* NodeA = NodeFromData(A);
			Node* NodeB = NodeFromData(B);

			T Temp = NodeA->Data;
			NodeA->Data = Move(NodeB->Data);
			NodeB->Data = Move(Temp);
		}

		bool Contains(const T& Other) const
		{
			for (Iterator It = Begin(); It != End(); It++)
			{
				if (*It == Other)
				{
					return true;
				}
			}

			return false;
		}

		uint64 GetCount() { return Count; }
		bool IsEmpty() { return Count == 0; }

	private:
		static Node* NodeFromData(T* Data)
		{
			return reinterpret_cast<Node*>(Data);
		}
		
		Node* CreateNode()
		{
			Count++;

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->ChildCount = 0;
			Instance->Parent = nullptr;
			Instance->Sibling = nullptr;
			Instance->Child = nullptr;
			return Instance;
		}

		void DestroyNode(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AddChild(Node* Parent, Node* Child)
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

			Parent->ChildCount++;
		}

		void InsertSibling(Node* Sibling, Node* Instance)
		{
			Instance->Parent = Sibling->Parent;
			Instance->Sibling = Sibling->Sibling;
			Sibling->Sibling = Instance;
			Sibling->Parent->ChildCount++;
		}

		void InsertChild(Node* Parent, Node* Instance)
		{
			Node* Child = Parent->Child;
			while (Child)
			{
				Child->Parent = Instance;
				Child = Child->Sibling;
			}

			Instance->Parent = Parent;
			Instance->Child = Parent->Child;
			Instance->ChildCount = Parent->ChildCount;
			Parent->Child = Instance;
			Parent->ChildCount = 1;
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
				Parent->ChildCount--;
			}

			DestroyNode(Instance);
		}

		void CopyNode(Node* Parent, Node* Instance)
		{
			Append(&Parent->Data, Instance->Data);

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
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Root;
	};
}