#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	class Queue
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

		Queue(Allocator* Allctr = nullptr)
			: Allocator(nullptr), Count(0), DataHead(nullptr), DataTail(nullptr)
		{
			ValidateAllocator(Allctr);
		}

		Queue(const Queue<T>& Other)
			: Allocator(Other.Allocator), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
		{
		}

		Queue(Queue<T>&& Other) noexcept
			: Allocator(Other.Allocator), Count(Other.Count), DataHead(Other.DataHead), DataTail(Other.DataTail)
		{
			Other.DataHead = nullptr;
			Other.DataTail = nullptr;
		}

		~Queue()
		{
			Clear();
		}

		Queue<T> Copy() const
		{
			Queue<T> Copy = Queue<T>(Allocator);

			Node* Current = DataHead;
			while (Current)
			{
				Copy.Append(Current->Value);
				Current = Current->Next;
			}

			return Copy;
		}

		bool operator==(const Queue<T>& Other) const
		{
			return Count == Other.Count && DataHead == Other.DataHead && DataTail == Other.DataTail;
		}

		bool operator!=(const Queue<T>& Other) const
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

		T& AppendRange(const Queue<T>& Value)
		{
			Node* Return = DataTail;

			Node* Current = Value.DataHead;
			while (Current)
			{
				Append(Current->Value);
				Current = Current->Next;
			}

			return Return->Next->Value;
		}

		void Remove()
		{
			NEXUS_ASSERT(!IsEmpty(), "Queue is empty");
			
			Node* Instance = DataHead;
			RemoveNode();
			Free(Instance);
		}

		void Clear()
		{
			while (DataHead)
			{
				Remove();
			}
		}

		T& Get() const
		{
			NEXUS_ASSERT(!IsEmpty(), "Queue is empty");
			
			return DataHead->Value;
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(&DataHead->Value);
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

		void Reverse()
		{
			DataTail = DataHead;

			Node* Current = DataHead;
			Node* Next = Current->Next;
			Current->Next = nullptr;

			while (Next)
			{
				Node* SecondNext = Next->Next;
				Next->Next = Current;

				Current = Next;
				Next = SecondNext;
			}

			DataHead = Current;
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

			Memory::Destruct(&Instance->Value);
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(Node* Instance)
		{
			if (DataHead == nullptr || DataTail == nullptr)
			{
				DataHead = DataTail = Instance;
				return;
			}

			DataTail->Next = Instance;
			DataTail = Instance;
		}

		void RemoveNode()
		{
			if (DataHead)
			{
				DataHead = DataHead->Next;
			}
			if (DataHead == nullptr)
			{
				DataTail = nullptr;
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
		Node* DataHead;
		Node* DataTail;
	};
}