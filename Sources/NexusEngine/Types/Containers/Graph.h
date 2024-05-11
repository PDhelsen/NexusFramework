#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Graph
	{
		enum class ConnectionType : uint8
		{
			From, To
		};

		struct Connection
		{
			ConnectionType Type;
			Connection* Next;
		};
		
		struct Node
		{
			T Data;
			uint64 Count;
			Node* Next;
			Connection* Connection;
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
				Pointer = &(NodeFromData(Pointer)->Next->Data);
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
				return Pointer;
			}

			T& operator*()
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

		Graph(Allocator* Alloc = nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
		}

		Graph(const Graph<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
		}

		Graph(Graph<T>&& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
			Other.Root = nullptr;
		}

		~Graph()
		{
			Clear();
		}

		bool operator==(const Graph<T>& Other)
		{
			return Count == Other.Count && Root == Other.Root;
		}

		bool operator!=(const Graph<T>& Other)
		{
			return Count != Other.Count || Root != Other.Root;
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

		void Append(const T& Value)
		{
			Node* New = CreateNode();
			New->Data = Value;

			AddNode(New);
		}

		void Append(T&& Value)
		{
			Node* New = CreateNode();
			New->Data = Move(Value);

			AddNode(New);
		}

		template<typename... Args>
		void Append(Args&&... args)
		{
			Node* New = CreateNode();
			Memory::Construct<T>(&New->Data, args...);

			AddNode(New);
		}

		void Append(const Graph<T>& Value)
		{
			Node* Copy = Value.Root;
			while (Copy)
			{
				Append(&Copy->Data);
				Copy = Copy->Next;
			}
		}

		void Remove(T* Value)
		{
			Node* Instance = NodeFromData(Value);
			RemoveNode(Instance);
			DestroyNode(Instance);
		}

		void Clear()
		{
			Node* Current = Root;
			while (Current)
			{
				Node* Next = Current->Next;
				DestroyNode(Current);
				Current = Next;
			}

			Root = nullptr;
		}

		T& GetRoot()
		{
			return Root->Data;
		}

		T* TryGetRoot()
		{
			return &Root->Data;
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
			Node* Node = Root;
			while (Node)
			{
				if (Node->Data == Other)
				{
					return true;
				}

				Node = Node->Next;
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
			New->Count = 0;
			New->Next = nullptr;
			New->Connection = nullptr;
			return New;
		}

		void DestroyNode(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AddNode(Node* Instance)
		{
			Instance->Next = Root;
			Root = Instance;
		}

		void RemoveNode(Node* Instance)
		{
			Node* Current = Root;
			while (Current) 
			{
				if (Current->Next == Instance)
				{
					Current->Next = Instance->Next;
					break;
				}

				Current = Current->Next;
			}

			if (Instance == Root)
			{
				Root = Instance->Next;
			}
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Root;
	};
}