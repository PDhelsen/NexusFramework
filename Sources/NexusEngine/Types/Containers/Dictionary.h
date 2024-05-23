#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"
#include "Misc/Hash.h"

namespace NxEn
{
	template<typename K, typename T, class H = Fnv1a64, float LF = 1.0f>
	class Dictionary
	{
	public:
		class KeyValuePair
		{
			friend Dictionary;

		public:
			const K& GetKey() const { return Key; }
			T& GetValue() { return Value; }

		private:
			K Key;
			T Value;
		};

	private:
		struct Node
		{
			KeyValuePair KeyValue;
			Node* Next;
		};

	public:
		class Iterator
		{
		public:
			Iterator(Node** Data, uint64 Capacity, Node* Current, uint64 Index)
				: Data(Data), Capacity(Capacity), Current(Current), Index(Index)
			{
				if (Current == nullptr && Index < Capacity)
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
				Iterator Copy = *this;
				++(*this);
				return Copy;
			}

			KeyValuePair* operator->()
			{
				return &Current->KeyValue;
			}

			KeyValuePair& operator*()
			{
				return Current->KeyValue;
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

		Dictionary(uint64 Cpct = DefaultCapacity, Allocator* Alloc = nullptr)
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

		Dictionary(const Dictionary<K, T, H, LF>& Other)
			: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
		}

		Dictionary(Dictionary<K, T, H, LF>&& Other) noexcept
			: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Dictionary()
		{
			Clear();
			Memory::Free(Data, Allocator);
		}

		Dictionary<K, T, H, LF> Copy() const
		{
			Dictionary<K, T, H, LF> Copy = Dictionary<K, T, H, LF>(Capacity, Allocator);

			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					Node* NodeCopy = Copy.CreateNode();
					NodeCopy->KeyValue.Key = Current->KeyValue.Key;
					NodeCopy->KeyValue.Value = Current->KeyValue.Value;
					Copy.AddNode(Index, NodeCopy);

					Current = Current->Next;
				}
			}

			return Copy;
		}

		T& operator[](const K& Key)
		{
			return Get(Key);
		}

		const T& operator[](const K& Key) const
		{
			return Get(Key);
		}

		bool operator==(const Dictionary<K, T, H, LF>& Other)
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dictionary<K, T, H, LF>& Other)
		{
			return Count != Other.Count || Data != Other.Data;
		}

		void Assign(const K& Key, const T& Value)
		{
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->KeyValue.Value = Value;
		}

		void Assign(const K& Key, T&& Value)
		{
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->KeyValue.Value = Move(Value);
		}

		template<typename... Args>
		void Assign(const K& Key, Args&&... args)
		{
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Memory::Construct<T>(&Instance->KeyValue.Value, args...);
		}

		void Append(const K& Key, const T& Value)
		{
			uint64 Index = GetIndex(Key);
			if (GetNode(Index, Key) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				ReHash(Grow());
			}

			Node* New = CreateNode();
			New->KeyValue.Key = Key;
			New->KeyValue.Value = Value;

			AddNode(Index, New);
		}

		void Append(const K& Key, T&& Value)
		{
			uint64 Index = GetIndex(Key);
			if (GetNode(Index, Key) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				ReHash(Grow());
			}

			Node* New = CreateNode();
			New->KeyValue.Key = Move(Key);
			New->KeyValue.Value = Move(Value);

			AddNode(Index, New);
		}

		template<typename... Args>
		void Append(const K& Key, Args&&... args)
		{
			uint64 Index = GetIndex(Key);
			if (GetNode(Index, Key) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				ReHash(Grow());
			}

			Node* New = CreateNode();
			New->KeyValue.Key = Key;
			Memory::Construct<T>(&New->KeyValue.Value, args...);

			AddNode(Index, New);
		}

		void Append(const Dictionary<K, T>& Other)
		{
			for (auto& Value : Other)
			{
				Append(Value);
			}
		}

		void Remove(const T& Key)
		{
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);

			NEXUS_ASSERT(Instance, "Key not in Dictionary");

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

		T& Get(const K& Key)
		{
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			return Instance->KeyValue.Value;
		}

		T* TryGet(const K& Key)
		{
			Node* Instance = GetNode(Key);
			if (!Instance)
			{
				return nullptr;
			}
			return &Instance->KeyValue.Value;
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
					uint64 Index = GetIndex(Current->KeyValue.Key);
					AddNode(Index, Current);

					Current = Next;
				}
			}

			Memory::Free(TempArray, Allocator);
		}

		void Swap(const K& A, const K& B)
		{
			Node* NodeA = GetNode(A);
			Node* NodeB = GetNode(B);

			NEXUS_ASSERT(NodeA, "Key A not in the Dictionary");
			NEXUS_ASSERT(NodeB, "Key B not in the Dictionary");

			T Temp = NodeA->KeyValue.Value;
			NodeA->KeyValue.Value = Move(NodeB->KeyValue.Value);
			NodeB->KeyValue.Value = Move(Temp);
		}

		bool ContainsKey(const K& Key) const
		{
			return GetNode(Key) != nullptr;
		}

		bool ContainsValue(const T& Other) const
		{
			return Find(Other) != nullptr;
		}

		const K* Find(const T& Other) const
		{
			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					if (Current->KeyValue.Value == Other)
					{
						return &Current->KeyValue.Key;
					}
					Current = Current->Next;
				}
			}

			return nullptr;
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

			if (Data[Index] == Instance)
			{
				Data[Index] = Instance->Next;
			}
		}

		uint64 GetIndex(const K& Key) const
		{
			uint64 Index = Hash<K, H>::HashObject(Key) % Capacity;
			NEXUS_ASSERT(IsValidIndex(Index), "Invalid Index");
			return Index;
		}

		Node* GetNode(uint64 Index, const K& Key) const
		{
			Node* Node = Data[Index];
			while (Node)
			{
				if (Node->KeyValue.Key == Key)
				{
					break;
				}

				Node = Node->Next;
			}

			return Node;
		}

		Node* GetNode(const K& Key) const
		{
			uint64 Index = GetIndex(Key);
			return GetNode(Index, Key);
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