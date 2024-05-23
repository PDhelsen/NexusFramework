#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"
#include "Misc/Hash.h"

namespace NxEn
{
	template<typename T, class H = Fnv1a64, float LF = 1.0f>
	class Set
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
			Iterator(Node** Data, uint64 Capacity, Node* Current, uint64 Index)
				: Data(Data), Capacity(Capacity), Current(Current), Index(Index)
			{
				if (Current == nullptr && Index != Capacity)
				{
					MoveToNext();
				}
			}

			Iterator& operator++()
			{
				MoveToNext();
				return *this;
			}

			Iterator operator++(int32)
			{
				Iterator Temp = *this;
				++(*this);
				return Temp;
			}

			const T* operator->()
			{
				return &Current->Value;
			}

			const T& operator*()
			{
				return Current->Value;
			}

			bool operator==(const Iterator& Other) const
			{
				return Current == Other.Current && Index == Other.Index;
			}

			bool operator!=(const Iterator& Other) const
			{
				return Current != Other.Current || Index != Other.Index;
			}

		private:
			void MoveToNext()
			{
				if (Current && Current->Next != nullptr)
				{
					Current = Current->Next;
				}
				else
				{
					do
					{
						Index++;
						if (Index >= Capacity)
						{
							Current = nullptr;
							break;
						}

						Current = Data[Index];
					} while (Current == nullptr);
				}
			}
			
			Node** Data;
			uint64 Capacity;
			Node* Current;
			uint64 Index;
		};

		Set(uint64 Cpct = DefaultCapacity, Allocator* Alloc = nullptr)
			: Allocator(nullptr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocator = Alloc != nullptr ? Alloc : Memory::GetActiveAllocator();
			Capacity = GetValidCapacity(Cpct);
			Data = (Node**)Memory::Allocate(sizeof(Node*) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
			for (uint64 Index = 0; Index <= Capacity; Index++)
			{
				Data[Index] = nullptr;
			}
		}

		Set(const Set<T, H, LF>& Other)
			: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
		}

		Set(Set<T, H, LF>&& Other) noexcept
			: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Set()
		{
			Clear();
			Memory::Free(Data, Allocator);
		}

		Set<T, H, LF> Copy() const
		{
			Set<T, H, LF> Copy = Set<T, H, LF>(Capacity, Allocator);
			
			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					Node* NodeCopy = Copy.CreateNode();
					NodeCopy->Value = Current->Value;
					Copy.AddNode(Index, NodeCopy);

					Current = Current->Next;
				}
			}

			return Copy;
		}

		bool operator==(const Set<T, H, LF>& Other)
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Set<T, H, LF>& Other)
		{
			return Count != Other.Count || Data != Other.Data;
		}

		void Append(const T& Value)
		{
			uint64 Index = GetIndex(Value);
			if (GetNode(Index, Value) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				ReHash(Grow());
			}
			
			Node* New = CreateNode();
			New->Value = Value;

			AddNode(Index, New);
		}

		void Append(T&& Value)
		{
			uint64 Index = GetIndex(Value);
			if (GetNode(Index, Value) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				ReHash(Grow());
			}

			Node* New = CreateNode();
			New->Value = Move(Value);

			AddNode(Index, New);
		}

		void Append(const Set<T>& Other)
		{
			for (auto& Value : Other)
			{
				Append(Value);
			}
		}

		void Remove(const T& Value)
		{
			uint64 Index = GetIndex(Value);
			Node* Instance = GetNode(Index, Value);

			NEXUS_ASSERT(Instance, "Value not in Set");

			RemoveNode(Index, Instance);
			DestroyNode(Instance);
		}

		void Clear()
		{
			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					Node* ToRemove = Current;
					Current = Current->Next;
					DestroyNode(ToRemove);
				}

				Data[Index] = nullptr;
			}
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, Capacity, Data[0], 0);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data, Capacity, nullptr, Capacity);
		}

		void ReHash(uint64 Size)
		{
			Node** TempArray = Data;
			uint64 TempCapacity = Capacity;

			Capacity = Size;
			Data = (Node**)Memory::Allocate(sizeof(Node*) * Capacity, NEXUS_MEMORY_ALIGN, Allocator);
			for (uint64 Index = 0; Index <= Capacity; Index++)
			{
				Data[Index] = nullptr;
			}

			for (uint64 TempIndex = 0; TempIndex < TempCapacity; TempIndex++)
			{
				Node* Current = TempArray[TempIndex];
				while (Current)
				{
					Node* Next = Current->Next;

					Current->Next = nullptr;
					uint64 Index = GetIndex(Current->Value);
					AddNode(Index, Current);

					Current = Next;
				}
			}

			Memory::Free(TempArray, Allocator);
		}

		bool Contains(const T& Other) const
		{
			return GetNode(Other) != nullptr;
		}

		float GetLoadFactor() const { return (float)(Count + 1) / (float)(Capacity); }
		float GetLoadFactorThrehsold() const { return LoadFactorThreshold; }
		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }
		bool IsEmpty() const { return Count == 0; }

	private:
		Node* CreateNode()
		{
			Count++;
			Node* New = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			New->Next = nullptr;
			return New;
		}

		void DestroyNode(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AddNode(uint64 Index, Node* Instance)
		{
			if (Data[Index] != nullptr)
			{
				Instance->Next = Data[Index];
			}

			Data[Index] = Instance;
		}

		void RemoveNode(uint64 Index, Node* Instance)
		{
			Node* Node = Data[Index];
			while (Node->Next)
			{
				if (Node->Next == Instance)
				{
					Node->Next = Instance->Next;
					break;
				}

				Node = Node->Next;
			}

			if (Instance == Data[Index])
			{
				Data[Index] = Instance->Next;
			}
		}

		uint64 GetIndex(const T& Value) const
		{
			uint64 Index = Hash<T, H>::HashObject(Value) % Capacity;
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			return Index;
		}

		Node* GetNode(uint64 Index, const T& Value) const
		{
			Node* Node = Data[Index];
			while (Node)
			{
				if (Node->Value == Value)
				{
					break;
				}

				Node = Node->Next;
			}

			return Node;
		}

		Node* GetNode(const T& Value)
		{
			uint64 Index = GetIndex(Value);
			return GetNode(Index, Value);
		}

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Capacity;
		}

		uint64 Grow()
		{
			return GetValidCapacity(Capacity + Capacity / 2);
		}

		uint64 GetValidCapacity(uint64 Size) const { return Size > 3 ? Size : 3; }

		inline static const uint64 DefaultCapacity = 16;
		inline static const float LoadFactorThreshold = LF;

		Allocator* Allocator;
		uint64 Capacity;
		uint64 Count;
		Node** Data;
	};
}