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
	class Stack
	{
	public:
		using Node = NodeSimple<T>;
		using Iterator = IteratorNodeSimple<T, Node>;

		Stack(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Stack(const Stack<T>& Other)
			: Allocator(Other.Allocator), Count(0), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Stack - Copy constructor");

			Node* Current = Other.Data;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			Reverse();
		}

		Stack(Stack<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Stack()
		{
			Clear();
		}

		Stack<T>& operator=(const Stack<T>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Stack - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Allocator = Other.Allocator;

			Node* Current = Other.Data;
			while (Current)
			{
				AppendBack(Current->Value);
				Current = Current->Next;
			}

			return *this;
		}

		Stack<T>& operator=(Stack<T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();

			Allocator = Other.Allocator;
			Count = Other.Count;
			Data = Other.Data;

			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
		}

		bool operator==(const Stack<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Stack<T>& Other) const
		{
			return !(*this == Other);
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
			typename C::Iterator It = Value.Begin();
			T& Return = Append(*It);

			for (++It; It != Value.End(); It++)
			{
				Append(*It);
			}

			return Return;
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), "Stack is empty");
			
			Node* Instance = Data;
			RemoveNode();
			Destruct(Instance);
			Free(Instance);
		}

		void Clear()
		{
			while (Data)
			{
				Remove();
			}
		}

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Stack is empty");
			
			return Data->Value;
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
			NEXUS_ASSERT(!IsEmpty(), "Stack is empty");
			NEXUS_ASSERT(A != nullptr, "A is null");
			NEXUS_ASSERT(B != nullptr, "B is null");

			T Temp = GetNode(A)->Value;
			GetNode(A)->Value = Move(GetNode(B)->Value);
			GetNode(B)->Value = Move(Temp);
		}

		void Reverse()
		{
			Node* Current = Data;
			Node* Next = Current->Next;
			Current->Next = nullptr;

			while (Next)
			{
				Node* SecondNext = Next->Next;
				Next->Next = Current;

				Current = Next;
				Next = SecondNext;
			}

			Data = Current;
		}

		void Sort(Sort::CompareFunction<T> Function = nullptr)
		{
			Data = Sort::LinkSort<T>(Data, Function);
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

	private:
		Node* Allocate()
		{
			Count++;

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Next = nullptr;
			return Instance;
		}

		void Free(Node* Instance)
		{
			Count--;

			Memory::Free(Instance, Allocator);
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

		void RemoveNode()
		{
			if (Data)
			{
				Data = Data->Next;
			}
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
