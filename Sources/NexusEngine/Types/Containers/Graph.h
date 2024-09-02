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
		using Node = NodeGraph<T>;
		using Connection = NodeGraphConnection<T>;
		using ConnectionType = NodeGraphConnectionType;
		using Iterator = IteratorNodeGraph<T, Node>;

		Graph(Allocator* Allctr = nullptr)
			: Alloc(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Graph(const Graph<T>& Other)
			: Alloc(Other.Alloc), Count(0), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Graph - Copy constructor");

			Node* Current = Other.Data;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			Current = Other.Data;
			while (Current)
			{
				Iterator From = Find(Current->Value);

				Connection* Link = Current->Connection;
				while (Link)
				{
					if (Link->Type == ConnectionType::To)
					{
						Iterator To = Find(Link->Target->Value);
						Connect(&(*From), &(*To));
					}

					Link = Link->Next;
				}

				Current = Current->Next;
			}
		}

		Graph(Graph<T>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Graph()
		{
			Clear();
		}

		Graph<T>& operator=(const Graph<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Graph - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Alloc = Other.Alloc;

			Node* Current = Other.Data;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			Current = Other.Data;
			while (Current)
			{
				Iterator From = Find(Current->Value);

				Connection* Link = Current->Connection;
				while (Link)
				{
					if (Link->Type == ConnectionType::To)
					{
						Iterator To = Find(Link->Target->Value);
						Connect(&(*From), &(*To));
					}

					Link = Link->Next;
				}

				Current = Current->Next;
			}

			return *this;
		}

		Graph<T>& operator=(Graph<T>&& Other) noexcept
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
			Destruct(Instance);
			Construct(Instance, args...);
			return Instance->Value;
		}

		T& Append(const T& Value)
		{
			Node* Instance = Allocate();
			Construct(Instance, Value);

			AppendNode(Instance);
			return Instance->Value;
		}

		T& Append(T&& Value)
		{
			Node* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNode(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			Node* Instance = Allocate();
			Construct(Instance, args...);

			AppendNode(Instance);
			return Instance->Value;
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); ++It)
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
			Destruct(Instance);
			Free(Instance);
		}

		void Clear()
		{
			Node* Current = Data;
			while (Current)
			{
				Node* Next = Current->Next;
				Destruct(Current);
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

					++Idx;
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
					
					++Idx;
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
				++Index;
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
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
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
			for (Iterator It = Begin(); It != End(); ++It)
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
			++Count;

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Alloc);
			Instance->Count = 0;
			Instance->Next = nullptr;
			Instance->Connection = nullptr;
			return Instance;
		}

		Connection* Allocate(Node* A, Node* B, ConnectionType Type)
		{
			++A->Count;

			Connection* Connect = (Connection*)Memory::Allocate(sizeof(Connection), NEXUS_MEMORY_ALIGN, Alloc);
			Connect->Target = B;
			Connect->Type = Type;
			Connect->Next = nullptr;
			return Connect;
		}

		void Free(Node* Instance)
		{
			--Count;

			Memory::Free(Instance, Alloc);
		}

		void Free(Connection* Instance, Node* A, Node* B)
		{
			--A->Count;

			Memory::Free(Instance, Alloc);
		}

		template<typename... Args>
		void Construct(Node* Instance, Args&&... args)
		{
			Memory::Construct<T>(&Instance->Value, args...);
		}

		void Destruct(Node* Instance)
		{
			Memory::Destruct(&Instance->Value);
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

			Connection* Connect = Allocate(A, B, Type);

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

			Free(Current, A, B);
		}

		static Node* GetNode(T* Value)
		{
			return reinterpret_cast<Node*>(Value);
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Alloc;
		uint64 Count;
		Node* Data;
	};
}
