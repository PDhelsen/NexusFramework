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
		Stack(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), Top(nullptr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		Stack(const Stack<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), Top(Other.Top)
		{
		}

		Stack(Stack<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), Top(Other.Top)
		{
			Other.Top = nullptr;
		}

		~Stack()
		{
			Clear();
		}

		Stack<T> Copy()
		{
			Stack<T> Copy = Stack<T>(Allocator);
			Node* Current = Top;
			while (Current)
			{
				Copy.Append(Current->Data);
				Current = Current->Next;
			}
			return Copy;
		}

		bool operator==(const Stack<T>& Other)
		{
			return Count == Other.Count && Top == Other.Top;
		}

		bool operator!=(const Stack<T>& Other)
		{
			return Count != Other.Count || Top != Other.Top;
		}

		void Append(const T& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Value;
			AppendNode(Instance);
		}

		void Append(T&& Value)
		{
			Node* Instance = CreateNode();
			Instance->Data = Move(Value);
			AppendNode(Instance);
		}

		template<typename... Args>
		void AppendConstruct(Args&&... args)
		{
			Node* Instance = CreateNode();
			Memory::Construct<T>(&Instance->Data, args...);
			AppendNode(Instance);
		}

		void AppendRange(const Stack<T>& Value)
		{
			Node* Current = Value.Top;
			while (Current)
			{
				Append(Current->Data);
				Current = Current->Next;
			}
		}

		void Remove()
		{
			Node* Instance = RemoveNode();
			DestroyNode(Instance);
		}

		void Clear()
		{
			while (Top)
			{
				Remove();
			}
		}

		T& Get() const
		{
			return Top->Data;
		}

		bool Contains(const T& Other) const
		{
			Node* Current = Top;
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
			Node* Current = Top;
			Node* Next = Current->Next;
			Current->Next = nullptr;

			while (Next)
			{
				Node* SecondNext = Next->Next;
				Next->Next = Current;

				Current = Next;
				Next = SecondNext;
			}

			Top = Current;
		}

		uint64 GetCount() const { return Count; }
		bool IsEmpty() const { return Count == 0; }

	private:
		static Node* NodeFromData(T* Data)
		{
			return reinterpret_cast<Node*>(Data);
		}

		Node* CreateNode()
		{
			Count++;
			return (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
		}

		void DestroyNode(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* Instance)
		{
			Instance->Next = Top;
			Top = Instance;
		}

		Node* RemoveNode()
		{
			Node* Instance = Top;
			if (Top)
			{
				Top = Top->Next;
			}
			return Instance;
		}

		Allocator* Allocator;
		uint64 Count;
		Node* Top;
	};
}