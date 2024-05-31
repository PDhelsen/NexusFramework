#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Stack
	{
		struct Node
		{
			T Value;
			Node* Next;
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
				Pointer = &(GetNode(Pointer)->Next->Value);
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

		Stack(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Stack(const Stack<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Data(Other.Data)
		{
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

		Stack<T> Copy() const
		{
			Stack<T> Copy = Stack<T>(Allocator);
			
			Node* Current = Data;
			while (Current)
			{
				Copy.Append(Current->Value);
				Current = Current->Next;
			}

			return Copy;
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

		T& AppendRange(const Stack<T>& Value)
		{
			Node* Return = Data;

			Node* Current = Value.Data;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			return Return->Next->Value;
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), "Stack is empty");
			
			Node* Instance = Data;
			RemoveNode();
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

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&Data->Value);
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
			return Find(Other) != nullptr;
		}

		T* Find(const T& Other) const
		{
			Node* Current = Data;
			while (Current != nullptr)
			{
				if (Current->Value == Other)
				{
					return &Current->Value;
				}

				Current = Current->Next;
			}

			return nullptr;
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

		uint64 GetCount() const { return Count; }
		bool IsEmpty() const { return Count == 0; }

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