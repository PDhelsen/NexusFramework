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
				Iterator Copy = *this;
				++(*this);
				return Copy;
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

		Tree(Allocator* Alloc = nullptr)
			: Allocator(nullptr), Count(0), Root(nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
		}

		Tree(const Tree<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
		}

		Tree(Tree<T>&& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
			Root = nullptr;
		}

		~Tree()
		{
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
		void Initialize(Args&&... args)
		{
			Root = CreateNode();
			Memory::Construct<T>(&Root->Data, args...);
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

		void Append(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");
			
			Node* New = CreateNode();
			New->Data = Value;

			Node* Instance = NodeFromData(Parent);
			AddChild(Instance, New);
		}

		void Append(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* New = CreateNode();
			New->Data = Move(Value);

			Node* Instance = NodeFromData(Parent);
			AddChild(Instance, New);
		}

		template<typename... Args>
		void Append(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			Node* Instance = NodeFromData(Parent);
			AddChild(Instance, New);
		}

		void AppendRange(T* Parent, const Tree<T>& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* Instance = NodeFromData(Parent);
			Node* Copy = NodeFromData(&Value.GetRoot());
			CopyNode(Instance, Copy);
		}

		void InsertSibling(T* Sibling, const T& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			
			Node* New = CreateNode();
			New->Data = Value;
		
			Node* Instance = NodeFromData(Sibling);
			InsertSibling(Instance, New);
		}

		void InsertSibling(T* Sibling, T&& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");
			
			Node* New = CreateNode();
			New->Data = Move(Value);

			Node* Instance = NodeFromData(Sibling);
			InsertSibling(Instance, New);
		}

		template<typename... Args>
		void InsertSibling(T* Sibling, Args&&... args)
		{
			NEXUS_ASSERT(Sibling != nullptr, "Sibling is null");

			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			Node* Instance = NodeFromData(Sibling);
			InsertSibling(Instance, New);
		}

		void InsertChild(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* New = CreateNode();
			New->Data = Value;

			Node* Instance = NodeFromData(Parent);
			InsertChild(Instance, New);
		}

		void InsertChild(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* New = CreateNode();
			New->Data = Move(Value);

			Node* Instance = NodeFromData(Parent);
			InsertChild(Instance, New);
		}

		template<typename... Args>
		void InsertChild(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, "Parent is null");

			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			Node* Instance = NodeFromData(Parent);
			InsertChild(Instance, New);
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

			Node* New = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			New->ChildCount = 0;
			New->Parent = nullptr;
			New->Sibling = nullptr;
			New->Child = nullptr;
			return New;
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

		void InsertSibling(Node* Sibling, Node* New)
		{
			New->Parent = Sibling->Parent;
			New->Sibling = Sibling->Sibling;
			Sibling->Sibling = New;
			Sibling->Parent->ChildCount++;
		}

		void InsertChild(Node* Parent, Node* New)
		{
			Node* Child = Parent->Child;
			while (Child)
			{
				Child->Parent = New;
				Child = Child->Sibling;
			}

			New->Parent = Parent;
			New->Child = Parent->Child;
			New->ChildCount = Parent->ChildCount;
			Parent->Child = New;
			Parent->ChildCount = 1;
		}

		void RemoveNode(Node* Instance)
		{
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