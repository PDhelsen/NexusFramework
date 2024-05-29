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
			T Data;
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
				Copy.Append(Current->Data);
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
			return Count != Other.Count || Data != Other.Data;
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

		void AppendRange(const Stack<T>& Value)
		{
			Node* Current = Value.Data;
			while (Current)
			{
				Append(Current->Data);
				Current = Current->Next;
			}
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), "Stack is empty");
			
			Node* Instance = RemoveNode();
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
			
			return Data->Data;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&Data->Data);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(nullptr);
		}

		bool Contains(const T& Other) const
		{
			Node* Current = Data;
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
			return (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
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

		Node* RemoveNode()
		{
			Node* Instance = Data;
			if (Data)
			{
				Data = Data->Next;
			}
			return Instance;
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
		Node* Data;
	};
}