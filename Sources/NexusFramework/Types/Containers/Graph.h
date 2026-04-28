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
	class Graph
	{
	public:
		using N = Node::NodeGraph<T>;
		using C = NxFr::Node::NodeGraphConnection<T>;
		using CT = NxFr::Node::NodeGraphConnectionType;
		using I = Iterator::IteratorNodeGraph<T, N>;

		Graph(Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Count(0), Data(nullptr)
		{
		}

		Graph(const Graph<T>& Other)
			: Alloc(Other.Alloc), Count(0), Data(nullptr)
		{
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
			Other.Count = 0;
			Other.Data = nullptr;
		}

		~Graph()
		{
			Clear();
		}

		Graph<T>& operator=(const Graph<T>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();

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

		bool operator==(const Graph<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Graph<T>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Graph<T>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Graph<T>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Graph<T>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Graph<T>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Assign(T* Position, const T& Value)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Item = Value;
			return Item;
		}

		T& Assign(T* Position, T&& Value)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Item = Move(Value);
			return Item;
		}

		template<typename... Args>
		T& AssignConstruct(T* Position, Args&&... args)
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			N* Instance = GetNode(Position);
			T& Item = GetItem(Instance);
			Destruct(Instance);
			Construct(Instance, args...);
			return Item;
		}

		T& Append(const T& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Value);

			AppendNode(Instance);
			return GetItem(Instance);
		}

		T& Append(T&& Value)
		{
			N* Instance = Allocate();
			Construct(Instance, Move(Value));

			AppendNode(Instance);
			return GetItem(Instance);
		}

		template<typename... Args>
		T& AppendConstruct(Args&&... args)
		{
			N* Instance = Allocate();
			Construct(Instance, args...);

			AppendNode(Instance);
			return GetItem(Instance);
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				Append(*It);
			}

			return GetItem(Data);
		}

		void Remove(T* Value)
		{
			NEXUS_ASSERT(Value != nullptr, Default, "Value is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			
			N* Instance = GetNode(Value);
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
			NEXUS_ASSERT(From != nullptr, Default, "From is null");
			NEXUS_ASSERT(To != nullptr, Default, "To is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			
			N* Start = GetNode(From);
			N* Target = GetNode(To);
			
			AppendConnection(Start, Target, CT::To);
			AppendConnection(Target, Start, CT::From);
		}

		void Disconnect(T* From, T* To)
		{
			NEXUS_ASSERT(From != nullptr, Default, "From is null");
			NEXUS_ASSERT(To != nullptr, Default, "To is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			N* Start = GetNode(From);
			N* Target = GetNode(To);

			RemoveConnection(Start, Target, CT::To);
			RemoveConnection(Target, Start, CT::From);
		}

		T& Get()
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			
			return GetItem(Data);
		}

		const T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			return GetItem(Data);
		}

		T& GetConnection(T* Position, CT Type, uint64 Index = 0) 
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			
			uint64 Idx = 0;
			N* Instance = GetNode(Position);

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

			NEXUS_ASSERT(Connect, Default, "Failed to find connection");
			return *Position;
		}

		const T& GetConnection(const T* Position, CT Type, uint64 Index = 0) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			uint64 Idx = 0;
			N* Instance = GetNode(Position);

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

			NEXUS_ASSERT(Connect, Default, "Failed to find connection");
			return *Position;
		}

		T* TryGetConnection(T* Position, CT Type, uint64 Index = 0) 
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			
			uint64 Idx = 0;
			N* Instance = GetNode(Position);
			
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

		const T* TryGetConnection(const T* Position, CT Type, uint64 Index = 0) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");

			uint64 Idx = 0;
			N* Instance = GetNode(Position);

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

		bool IsConnected(const T* Position, const T* To, CT Type) const
		{
			uint64 Index = 0;
			return IsConnected(Position, To, Type, Index);
		}
		
		bool IsConnected(const T* Position, const T* To, CT Type, uint64& Index) const
		{
			NEXUS_ASSERT(Position != nullptr, Default, "Position is null");
			NEXUS_ASSERT(To != nullptr, Default, "To is null");
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			
			Index = 0;
			const N* Instance = GetNode(Position);
			const N* Target = GetNode(To);

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
			return GetIteratorNode(GetNode(Position));
		}

		const I GetIterator(T* Position) const
		{
			return GetIteratorNode(GetNode(Position));
		}

		I begin() { return Begin(); }
		I Begin() 
		{
			return GetIteratorNode(Data);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIteratorNode(Data);
		}

		I end() { return End(); }
		I End()
		{
			return GetIteratorNode(nullptr);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIteratorNode(nullptr);
		}

		void Swap(T* A, T* B)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Graph is empty");
			NEXUS_ASSERT(A != nullptr, Default, "A is null");
			NEXUS_ASSERT(B != nullptr, Default, "B is null");

			T Temp = GetItem(GetNode(A));
			GetItem(GetNode(A)) = Move(GetItem(GetNode(B)));
			GetItem(GetNode(B)) = Move(Temp);
		}

		bool Contains(const T& Other) const { return Contains([&](const T& Element) { return Element == Other; }); }
		bool Contains(const Iterator::Predicate<T>& Predicate) const
		{
			return GetIteratorValue(Predicate) != End();
		}

		I Find(const T& Other) { return Find([&](const T& Element) { return Element == Other; }); }
		I Find(const Iterator::Predicate<T>& Predicate)
		{
			return GetIteratorValue(Predicate);
		}

		const I Find(const T& Other) const { return Find([&](const T& Element) { return Element == Other; }); }
		const I Find(const Iterator::Predicate<T>& Predicate) const
		{
			return GetIteratorValue(Predicate);
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetConnectionCount(T* Instance) const { return GetNode(Instance)->Count; }

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

		I GetIteratorNode(N* Instance)
		{
			return I(Instance);
		}

		const I GetIteratorNode(const N* Instance) const
		{
			return I(const_cast<N*>(Instance));
		}

		I GetIteratorValue(const Iterator::Predicate<T>& Predicate) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (Predicate(*It))
				{
					return It;
				}
			}

			return End();
		}

		Allocator* Alloc;
		uint64 Count;
		N* Data;
	};
}
