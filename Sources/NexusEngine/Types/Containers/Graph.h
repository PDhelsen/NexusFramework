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
				Pointer = &(GetNode(Pointer)->Next->Data);
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
				return Pointer;
			}

			T& operator*() const
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

		Graph(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Root(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Graph(const Graph<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
		}

		Graph(Graph<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Root(Other.Root)
		{
			Other.Root = nullptr;
		}

		~Graph()
		{
			Clear();
		}

		bool operator==(const Graph<T>& Other) const
		{
			return Count == Other.Count && Root == Other.Root;
		}

		bool operator!=(const Graph<T>& Other) const
		{
			return Count != Other.Count || Root != Other.Root;
		}

		void Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Instance = GetNode(Position);
			Instance->Data = Value;
		}

		void Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Instance = GetNode(Position);
			Instance->Data = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Instance = GetNode(Position);
			Memory::Construct<T>(&Instance->Data, args...);
		}

		void Append(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Value;

			AppendNode(Instance);
		}

		void Append(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Data = Move(Value);

			AppendNode(Instance);
		}

		template<typename... Args>
		void AppendConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Data, args...);

			AppendNode(Instance);
		}

		void AppendRange(const Graph<T>& Value)
		{
			Node* Current = Value.Root;
			while (Current)
			{
				Append(&Current->Data);
				Current = Current->Next;
			}
		}

		void Remove(T* Value)
		{
			NEXUS_ASSERT(Value != nullptr, "Value is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			Node* Instance = GetNode(Value);
			RemoveNode(Instance);
			Free(Instance);
		}

		void Clear()
		{
			Node* Current = Root;
			while (Current)
			{
				Node* Next = Current->Next;
				Free(Current);
				Current = Next;
			}

			Root = nullptr;
		}

		void Connect(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			Node* Start = GetNode(From);
			Node* Target = GetNode(To);
			
			AppendConnection(Start, Target, ConnectionType::To);
			AppendConnection(Target, Start, ConnectionType::From);
		}

		void Disconnect(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Start = GetNode(From);
			Node* Target = GetNode(To);

			RemoveConnection(Start, Target, ConnectionType::To);
			RemoveConnection(Target, Start, ConnectionType::From);
		}

		T& GetRoot() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
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

		T& GetConnection(T* Instance, ConnectionType Type, uint64 Position = 0) const
		{
			NEXUS_ASSERT(Instance != nullptr, "Instance is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			uint64 Idx = 0;
			Node* Start = GetNode(Instance);

			Connection* Connect = Start->Connection;
			while (Connect)
			{
				if (Connect->Type == Type)
				{
					if (Idx == Position)
					{
						break;
					}

					Idx++;
				}

				Connect = Connect->Next;
			}

			return Connect->Target->Data;
		}

		T* TryGetConnection(T* Instance, ConnectionType Type, uint64 Position = 0) const
		{
			NEXUS_ASSERT(Instance != nullptr, "Instance is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			uint64 Idx = 0;
			Node* Start = GetNode(Instance);
			
			Connection* Connect = Start->Connection;
			while (Connect)
			{
				if (Connect->Type == Type)
				{
					if (Idx == Position)
					{
						return &Connect->Target->Data;
					}
					
					Idx++;
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
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null"); 
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			Node* NodeA = GetNode(A);
			Node* NodeB = GetNode(B);

			T Temp = NodeA->Data;
			NodeA->Data = Move(NodeB->Data);
			NodeB->Data = Move(Temp);
		}

		bool Contains(const T& Other) const
		{
			Node* Current = Root;
			while (Current)
			{
				if (Current->Data == Other)
				{
					return true;
				}

				Current = Current->Next;
			}

			return false;
		}

		uint64 GetConnectionCount(T* Instance) const { return GetNode(Instance)->Count; }
		uint64 GetCount() const { return Count; }
		bool IsEmpty() const { return Count == 0; }

	private:
		Node* Allocate()
		{
			Count++;

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Count = 0;
			Instance->Next = nullptr;
			Instance->Connection = nullptr;
			return Instance;
		}

		void Free(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* Instance)
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

		void AppendConnection(Node* A, Node* B, ConnectionType Type)
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

		static Node* GetNode(T* Data)
		{
			return reinterpret_cast<Node*>(Data);
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Root;
	};
}