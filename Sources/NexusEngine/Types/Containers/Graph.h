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
		using N = Node::NodeGraph<T>;
		using C = NxEn::Node::NodeGraphConnection<T>;
		using CT = NxEn::Node::NodeGraphConnectionType;
		using I = Iterator::IteratorNodeGraph<T, N>;

		Graph(Allocator* Allctr = nullptr)
			: Alloc(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Graph(const Graph<T>& Other)
			: Alloc(Other.Alloc), Count(0), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Graph - Copy constructor");

			N* Current = Other.Data;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			Current = Other.Data;
			while (Current)
			{
				I From = Find(Current->Value);

				C* Link = Current->Connection;
				while (Link)
				{
					if (Link->Type == CT::To)
					{
						I To = Find(Link->Target->Value);
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

			N* Current = Other.Data;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			Current = Other.Data;
			while (Current)
			{
				I From = Find(Current->Value);

				C* Link = Current->Connection;
				while (Link)
				{
					if (Link->Type == CT::To)
					{
						I To = Find(Link->Target->Value);
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

			N* Instance = Node::GetNode<T, N>(Position);
			Instance->Value = Value;
			return Instance->Value;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Instance->Value = Move(Value);
			return Instance->Value;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			N* Instance = Node::GetNode<T, N>(Position);
			Destruct(Instance);
			Construct(Instance, args...);
			return Instance->Value;
		}

		T& Append(const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			AppendNode(Instance);
			return Instance->Value;
		}

		T& Append(T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNode(Instance);
			return Instance->Value;
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			AppendNode(Instance);
			return Instance->Value;
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				Append(*It);
			}

			return Data->Value;
		}

		void Remove(T* Value)
		{
			NEXUS_ASSERT(Value != nullptr, "Value is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			N* Instance = Node::GetNode<T, N>(Value);
			RemoveNode(Instance);
			Destruct(Instance);
			Free(Instance);
		}

		void Clear()
		{
			N* Current = Data;
			while (Current)
			{
				N* Next = Current->Next;
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
			
			N* Start = Node::GetNode<T, N>(From);
			N* Target = Node::GetNode<T, N>(To);
			
			AppendConnection(Start, Target, CT::To);
			AppendConnection(Target, Start, CT::From);
		}

		void Disconnect(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, "From is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");

			N* Start = Node::GetNode<T, N>(From);
			N* Target = Node::GetNode<T, N>(To);

			RemoveConnection(Start, Target, CT::To);
			RemoveConnection(Target, Start, CT::From);
		}

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			return Data->Value;
		}

		T& GetConnection(T* Position, CT Type, uint64 Index = 0) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			uint64 Idx = 0;
			N* Instance = Node::GetNode<T, N>(Position);

			C* Connect = Instance->Connection;
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

		T* TryGetConnection(T* Position, CT Type, uint64 Index = 0) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			uint64 Idx = 0;
			N* Instance = Node::GetNode<T, N>(Position);
			
			C* Connect = Instance->Connection;
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

		bool IsConnected(T* Position, T* To, CT Type) const
		{
			uint64 Index = 0;
			return IsConnected(Position, To, Type, Index);
		}
		
		bool IsConnected(T* Position, T* To, CT Type, uint64& Index) const
		{
			NEXUS_ASSERT(Position != nullptr, "Position is null");
			NEXUS_ASSERT(To != nullptr, "To is null");
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
			
			Index = 0;
			N* Instance = Node::GetNode<T, N>(Position);
			N* Target = Node::GetNode<T, N>(To);

			C* Connect = Instance->Connection;
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
			NEXUS_ASSERT(!IsEmpty(), "Graph is empty");
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
		uint64 GetConnectionCount(T* Instance) const { return Node::GetNode<T, N>(Instance)->Count; }

	private:
		N* Allocate()
		{
			++Count;

			N* Instance = (N*)Memory::Allocate(sizeof(N), Alloc);
			Instance->Count = 0;
			Instance->Next = nullptr;
			Instance->Connection = nullptr;
			return Instance;
		}

		C* Allocate(N* A, N* B, CT Type)
		{
			++A->Count;

			C* Connect = (C*)Memory::Allocate(sizeof(C), Alloc);
			Connect->Target = B;
			Connect->Type = Type;
			Connect->Next = nullptr;
			return Connect;
		}

		void Free(N* Instance)
		{
			--Count;

			Memory::Free(Instance, Alloc);
		}

		void Free(C* Instance, N* A, N* B)
		{
			--A->Count;

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

		void AppendNode(N* Instance)
		{
			Instance->Next = Data;
			Data = Instance;
		}

		void RemoveNode(N* Instance)
		{
			N* Current = Data;
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

		void AppendConnection(N* A, N* B, CT Type)
		{
			C* Prev = nullptr;
			C* Current = A->Connection;

			while (Current)
			{
				if (Current->Target == B && Current->Type == Type)
				{
					return;
				}

				Prev = Current;
				Current = Current->Next;
			}

			C* Connect = Allocate(A, B, Type);

			if (Prev)
			{
				Prev->Next = Connect;
			}
			else
			{
				A->Connection = Connect;
			}
		}

		void RemoveConnection(N* A, N* B, CT Type)
		{
			C* Prev = nullptr;
			C* Current = A->Connection;
			
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

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Allocator* Alloc;
		uint64 Count;
		N* Data;
	};
}
