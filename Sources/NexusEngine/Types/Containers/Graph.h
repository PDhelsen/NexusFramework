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
	class Graph
	{
	public:
		using Node = LinkedNodeGraph<T>;
		using Connection = LinkedConnectionGraph<T>;
		using ConnectionType = LinkedConnectionType;
		using Iterator = LinkedIteratorGraph<T, Node>;

		Graph(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Graph(const Graph<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
		}

		Graph(Graph<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Graph()
		{
			Clear();
		}

		Graph<T> Copy() const
		{
			Graph<T> Copy = Graph<T>(Allocator);
			
			Node* Current = Data;
			while (Current)
			{
				Copy.Append(Current->Value);
				Current = Current->Next;
			}

			Current = Data;
			while (Current)
			{
				Iterator Data = Copy.Find(Current->Value);

				Connection* Connect = Current->Connection;
				while (Connect)
				{
					if (Connect->Type == ConnectionType::To)
					{
						Iterator Target = Copy.Find(Connect->Target->Value);
						Copy.Connect(&(*Data), &(*Target));
					}

					Connect = Connect->Next;
				}
				
				Current = Current->Next;
			}

			return Copy;
		}

		bool operator==(const Graph<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Graph<T>& Other) const
		{
			return !(*this == Other);
		}

		T& Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Instance = GetNode(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			Node* Instance = GetNode(Position);
			Memory::Construct<T>(&Instance->Value, args...);
			return Instance->Value;
		}

		T& Append(const T& Value)
		{
			Node* Instance = Allocate();
			Instance->Value = Value;

			AppendNode(Instance);
			return Instance->Value;
		}

		T& Append(T&& Value)
		{
			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			AppendNode(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Memory::Construct<T>(&Instance->Value, args...);

			AppendNode(Instance);
			return Instance->Value;
		}

		T& AppendRange(const Graph<T>& Value)
		{
			for (Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				Append(*It);
			}

			return Data->Value;
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
			Node* Current = Data;
			while (Current)
			{
				Node* Next = Current->Next;
				Free(Current);
				Current = Next;
			}

			Data = nullptr;
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

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			return Data->Value;
		}

		T& GetConnection(T* Position, ConnectionType Type, uint64 Index = 0) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			uint64 Idx = 0;
			Node* Instance = GetNode(Position);

			Connection* Connect = Instance->Connection;
			while (Connect)
			{
				if (Connect->Type == Type)
				{
					if (Idx == Index)
					{
						return Connect->Target->Value;
					}

					Idx++;
				}

				Connect = Connect->Next;
			}

			NEXUS_ASSERT(Connect, "Failed to find connection")
			return *Position;
		}

		T* TryGetConnection(T* Position, ConnectionType Type, uint64 Index = 0) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			uint64 Idx = 0;
			Node* Instance = GetNode(Position);
			
			Connection* Connect = Instance->Connection;
			while (Connect)
			{
				if (Connect->Type == Type)
				{
					if (Idx == Index)
					{
						return &Connect->Target->Value;
					}
					
					Idx++;
				}

				Connect = Connect->Next;
			}

			return nullptr;
		}

		bool IsConnected(T* Position, T* To, ConnectionType Type) const
		{
			uint64 Index = 0;
			return IsConnected(Position, To, Type, Index);
		}
		
		bool IsConnected(T* Position, T* To, ConnectionType Type, uint64& Index) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			Index = 0;
			Node* Instance = GetNode(Position);
			Node* Target = GetNode(To);

			Connection* Connect = Instance->Connection;
			while (Connect)
			{
				if (Connect->Target == Target && Connect->Type == Type)
				{
					return true;
				}

				Connect = Connect->Next;
				Index++;
			}

			return false;
		}

		Iterator GetIterator(T* Position)
		{
			return Iterator(GetNode(Position));
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data);
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
			return Find(Other) != End();
		}

		Iterator Find(const T& Other) const
		{
			for (Iterator It = Begin(); It != End(); It++)
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
		uint64 GetConnectionCount(T* Instance) const { return GetNode(Instance)->Count; }

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

			Memory::Destruct(&Instance->Value);
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* Instance)
		{
			Instance->Next = Data;
			Data = Instance;
		}

		void RemoveNode(Node* Instance)
		{
			Node* Current = Data;
			while (Current) 
			{
				if (Current->Next == Instance)
				{
					Current->Next = Instance->Next;
					break;
				}

				Current = Current->Next;
			}

			if (Instance && Instance == Data)
			{
				Data = Instance->Next;
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