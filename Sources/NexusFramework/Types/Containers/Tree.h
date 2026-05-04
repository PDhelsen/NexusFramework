#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Types/Containers/Node.h"
#include "NexusFramework/Types/Containers/Iterator.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename T>
	class Tree
	{
		friend class ContainersUtils;

	public:
		using N = Node::NodeTree<T>;
		using I = Iterator::IteratorNodeTree<T, N>;

		Tree(Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Count(0), Data(nullptr)
		{
		}

		Tree(const Tree<T>& Other)
			: Alloc(Other.Alloc), Count(0), Data(nullptr)
		{
			AppendRange(nullptr, Other);
		}

		Tree(Tree<T>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
		{
			Other.Count = 0;
			Other.Data = nullptr;
		}

		~Tree()
		{
			Clear();
		}

		Tree<T>& operator=(const Tree<T>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

			AppendRange(nullptr, Other);

			return *this;
		}

		Tree<T>& operator=(Tree<T>&& Other) noexcept
		{
			if (this == &Other)
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

		bool operator>(const Tree<T>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Tree<T>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Tree<T>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Tree<T>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Item = Value;
			return Item;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Item = Move(Value);
			return Item;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Destruct(Instance);
			Construct(Instance, args...);
			return Item;
		}

		T& Append(T* Parent, const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = GetNode(Parent);
			AppendNode(Anchor, Instance);
			return GetItem(Instance);
		}

		T& Append(T* Parent, T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = GetNode(Parent);
			AppendNode(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& AppendConstruct(T* Parent, Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = GetNode(Parent);
			AppendNode(Anchor, Instance);
			return GetItem(Instance);
		}

		T& AppendRange(T* Parent, const Tree<T>& Value)
		{
			N* Anchor = GetNode(Parent);
			const N* Copy = GetNode(&Value.Get());
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
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = GetNode(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return GetItem(Instance);
		}

		T& InsertSibling(T* Sibling, T&& Value)
		{
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = GetNode(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& InsertSiblingConstruct(T* Sibling, Args&&... args)
		{
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = GetNode(Sibling);
			InsertNodeSibling(Anchor, Instance);
			return GetItem(Instance);
		}

		T& InsertChild(T* Parent, const T& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Value);

			N* Anchor = GetNode(Parent);
			InsertNodeChild(Anchor, Instance);
			return GetItem(Instance);
		}

		T& InsertChild(T* Parent, T&& Value)
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			N* Anchor = GetNode(Parent);
			InsertNodeChild(Anchor, Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& InsertChildConstruct(T* Parent, Args&&... args)
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = Allocate();
			Construct(Instance, args...);

			N* Anchor = GetNode(Parent);
			InsertNodeChild(Anchor, Instance);
			return GetItem(Instance);
		}

		void Remove(T* Root)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			N* Instance = GetNode(Root);
			RemoveNode(Instance);
		}

		void RemoveChildren(T* Root)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			N* Instance = GetNode(Root);
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

		T& Get()  
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetItem(Data);
		}

		const T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return GetItem(Data);
		}

		T& GetParent(T* Child)
		{
			NEXUS_ASSERT(Child != nullptr, Default, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetItem(GetNode(Child)->Parent);
		}

		const T& GetParent(const T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, Default, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return GetItem(GetNode(Child)->Parent);
		}

		T* TryGetParent(T* Child)
		{
			NEXUS_ASSERT(Child != nullptr, Default, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Child);
			if (!Instance->Parent)
			{
				return nullptr;
			}
			return &GetItem(Instance->Parent);
		}

		const T* TryGetParent(const T* Child) const
		{
			NEXUS_ASSERT(Child != nullptr, Default, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Child);
			if (!Instance->Parent)
			{
				return nullptr;
			}
			return &GetItem(Instance->Parent);
		}

		T& GetSibling(T* Sibling)
		{
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetItem(GetNode(Sibling)->Sibling);
		}

		const T& GetSibling(const T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return GetItem(GetNode(Sibling)->Sibling);
		}

		T* TryGetSibling(T* Sibling)
		{
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Sibling);
			if (!Instance->Sibling)
			{
				return nullptr;
			}
			return &GetItem(Instance->Sibling);
		}

		const T* TryGetSibling(const T* Sibling) const
		{
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Sibling);
			if (!Instance->Sibling)
			{
				return nullptr;
			}
			return &GetItem(Instance->Sibling);
		}

		T& GetChild(T* Parent)
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetItem(GetNode(Parent)->Child);
		}

		const T& GetChild(const T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return GetItem(GetNode(Parent)->Child);
		}

		T* TryGetChild(T* Parent)
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Parent);
			if (!Instance->Child)
			{
				return nullptr;
			}
			return &GetItem(Instance->Child);
		}

		const T* TryGetChild(const T* Parent) const
		{
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			N* Instance = GetNode(Parent);
			if (!Instance->Child)
			{
				return nullptr;
			}
			return &GetItem(Instance->Child);
		}

		bool IsParent(const T* Position, const T* Parent) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(Parent != nullptr, Default, "Parent is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetNode(Position)->Parent == GetNode(Parent);
		}

		bool IsSibling(const T* Position, const T* Sibling) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(Sibling != nullptr, Default, "Sibling is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetNode(Position)->Parent == GetNode(Sibling)->Parent;
		}

		bool IsChild(const T* Position, const T* Child) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(Child != nullptr, Default, "Child is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");
			
			return GetNode(Position) == GetNode(Child)->Parent;
		}

		bool IsConnected(const T* A, const T* B) const
		{
			NEXUS_ASSERT(A != nullptr, Default, "A is null");
			NEXUS_ASSERT(B != nullptr, Default, "B is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return IsParent(A, B) || IsSibling(A, B) || IsChild(A, B);
		}

		I GetIterator(T* Position)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return GetIt(GetNode(Position));
		}

		const I GetIterator(const T* Position) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Tree is empty");

			return GetIt(GetNode(Position));
		}

		I begin() { return Begin(); }
		I Begin() 
		{
			return GetIt(Data);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIt(Data);
		}

		I end() { return End(); }
		I End() 
		{
			return GetIt(nullptr);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIt(nullptr);
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		bool IsInitialized() const { Data != nullptr; }
		uint64 GetChildCount(T* Instance) const { return GetNode(Instance)->Count; }
		uint64 GetSiblingCount(T* Instance) const { return GetNode(Instance)->Parent->Count - 1; }

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
				NEXUS_ASSERT(false, Default, "Trying to replace root of the tree is not allowed");
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

		T& CopyNode(N* Parent, const N* Instance)
		{
			T& Return = Append(Parent != nullptr ? &Parent->Value : nullptr, Instance->Value);

			N* Copy = GetNode(&Return);
			N* Child = Instance->Child;
			while (Child)
			{
				CopyNode(Copy, Child);
				Child = Child->Sibling;
			}

			return Return;
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

		I GetIt(N* Instance)
		{
			return I(Instance);
		}

		const I GetIt(const N* Instance) const
		{
			return I(const_cast<N*>(Instance));
		}

		Allocator* Alloc;
		uint64 Count;
		N* Data;
	};
}
