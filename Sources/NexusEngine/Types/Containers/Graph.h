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
		struct Connection;
		struct Node;

	public:
		enum class ConnectionType : uint8
		{
			From, To
		};

	private:
		struct Connection
		{
			Node* Target;
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
				Iterator Temp = *this;
				++(*this);
				return Temp;
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
			: Allocator(nullptr), Count(0), Root(nullptr)
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

		void Connect(T* From, T* To)
		{
			Node* Start = NodeFromData(From);
			Node* Target = NodeFromData(To);
			
			AddConnection(Start, Target, ConnectionType::To);
			AddConnection(Target, Start, ConnectionType::From);
		}

		void Disconnect(T* From, T* To)
		{
			Node* Start = NodeFromData(From);
			Node* Target = NodeFromData(To);

			RemoveConnection(Start, Target, ConnectionType::To);
			RemoveConnection(Target, Start, ConnectionType::From);
		}

		T& GetRoot()
		{
			return Root->Data;
		}

		T* TryGetRoot()
		{
			return &Root->Data;
		}

		T* TryGetConnection(T* Instance, ConnectionType Type, uint64 Index = 0)
		{
			uint64 I = 0;
			Node* Start = NodeFromData(Instance);
			
			Connection* Connect = Start->Connection;
			while (Connect)
			{
				if (Connect->Type == Type)
				{
					if (I == Index)
					{
						return &Connect->Target->Data;
					}
					
					I++;
				}

				Connect = Connect->Next;
			}

			return nullptr;
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

		uint64 GetConnectionCount(T* Instance) { return NodeFromData(Instance)->Count; }
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

			if (Instance && Instance == Root)
			{
				Root = Instance->Next;
			}
		}

		void AddConnection(Node* A, Node* B, ConnectionType Type)
		{
			Connection* Prev = nullptr;
			Connection* Current = A->Connection;

			while (Current)
			{
				if (Current->Target == B && Current->Type == Type)
				{
					return;
				}

				Prev = Current;
				Current = Current->Next;
			}

			Connection* Connect = (Connection*)Memory::Allocate(sizeof(Connection), NEXUS_MEMORY_ALIGN, Allocator);
			Connect->Target = B;
			Connect->Type = Type;
			Connect->Next = nullptr;
			A->Count++;

			if (Prev)
			{
				Prev->Next = Connect;
			}
			else
			{
				A->Connection = Connect;
			}
		}

		void RemoveConnection(Node* A, Node* B, ConnectionType Type)
		{
			Connection* Prev = nullptr;
			Connection* Current = A->Connection;
			
			while (Current)
			{
				if (Current->Target == B && Current->Type == Type)
				{
					break;
				}

				Prev = Current;
				Current = Current->Next;
			}

			if (!Current)
			{
				return;
			}

			if (Prev)
			{
				Prev->Next = Current->Next;
			}
			else
			{
				A->Connection = Current->Next;
			}

			Memory::Free(Current, Allocator);
			A->Count--;
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Root;
	};
}