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
			Iterator(Node** Data, uint64 Buckets, Node* Current, uint64 Index)
				: Data(Data), Buckets(Buckets), Current(Current), Index(Index)
			{
				if (Current == nullptr && Index < Buckets)
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

			KeyValuePair* operator->() const
			{
				return &Current->KeyValue;
			}

			KeyValuePair& operator*() const
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
						if (Index >= Buckets)
						{
							Current = nullptr;
							break;
						}

						Current = Data[Index];
					} while (Current == nullptr);
				}
			}

			Node** Data;
			uint64 Buckets;
			Node* Current;
			uint64 Index;
		};

		Dictionary(uint64 Size = DefaultSize, Allocator* Allctr = nullptr)
			: Allocator(nullptr), Buckets(0), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
		}

		Dictionary(const Dictionary<K, T, H, LF>& Other)
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), Data(Other.Data)
		{
		}

		Dictionary(Dictionary<K, T, H, LF>&& Other) noexcept
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Dictionary()
		{
			Clear();
			Free(Data);
		}

		Dictionary<K, T, H, LF> Copy() const
		{
			Dictionary<K, T, H, LF> Copy = Dictionary<K, T, H, LF>(Buckets, Allocator);

			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					Node* NodeCopy = Copy.Allocate();
					NodeCopy->KeyValue.Key = Current->KeyValue.Key;
					NodeCopy->KeyValue.Value = Current->KeyValue.Value;
					Copy.AppendNode(Index, NodeCopy);

					Current = Current->Next;
				}
			}

			return Copy;
		}

		T& operator[](const K& Key) const
		{
			return Get(Key);
		}

		bool operator==(const Dictionary<K, T, H, LF>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dictionary<K, T, H, LF>& Other) const
		{
			return !(*this == Other);
		}

		void Assign(const K& Key, const T& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->KeyValue.Value = Value;
		}

		void Assign(const K& Key, T&& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->KeyValue.Value = Move(Value);
		}

		template<typename... Args>
		void AssignConstruct(const K& Key, Args&&... args)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
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
				Resize(Buckets + Buckets / 2);
			}

			Node* New = Allocate();
			New->KeyValue.Key = Key;
			New->KeyValue.Value = Value;

			AppendNode(Index, New);
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
				Resize(Buckets + Buckets / 2);
			}

			Node* New = Allocate();
			New->KeyValue.Key = Move(Key);
			New->KeyValue.Value = Move(Value);

			AppendNode(Index, New);
		}

		template<typename... Args>
		void AppendConstruct(const K& Key, Args&&... args)
		{
			uint64 Index = GetIndex(Key);
			if (GetNode(Index, Key) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}

			Node* New = Allocate();
			New->KeyValue.Key = Key;
			Memory::Construct<T>(&New->KeyValue.Value, args...);

			AppendNode(Index, New);
		}

		void AppendRange(const Dictionary<K, T, H, LF>& Other)
		{
			for (auto& Kv : Other)
			{
				Append(Kv.GetKey(), Kv.GetValue());
			}
		}

		void Remove(const T& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);

			NEXUS_ASSERT(Instance, "Key not in Dictionary");

			RemoveNode(Index, Instance);
			Free(Instance);
		}

		void Clear()
		{
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					Node* ToRemove = Current;
					Current = Current->Next;
					Free(ToRemove);
				}

				Data[Index] = nullptr;
			}
		}

		T& Get(const K& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			return Instance->KeyValue.Value;
		}

		T* TryGet(const K& Key) const
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
			return Iterator(Data, Buckets, Data[0], 0);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data, Buckets, nullptr, Buckets);
		}

		void Resize(uint64 Size)
		{
			Node** TempArray = Data;
			uint64 TempCapacity = Buckets;

			Allocate(Size);

			for (uint64 TempIndex = 0; TempIndex < TempCapacity; TempIndex++)
			{
				Node* Current = TempArray[TempIndex];
				while (Current)
				{
					Node* Next = Current->Next;

					Current->Next = nullptr;
					uint64 Index = GetIndex(Current->KeyValue.Key);
					AppendNode(Index, Current);

					Current = Next;
				}
			}

			Free(TempArray);
		}

		void Swap(const K& A, const K& B)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
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
			return FindKey(Key) != nullptr;
		}

		bool ContainsValue(const T& Other) const
		{
			return FindValue(Other) != nullptr;
		}

		const K* FindKey(const K& Key) const
		{
			Node* Instance = GetNode(Key);
			if (!Instance)
			{
				return nullptr;
			}

			return &Instance->KeyValue.Key;
		}

		const K* FindValue(const T& Other) const
		{
			for (uint64 Index = 0; Index < Buckets; Index++)
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

		float GetLoadFactor() const { return (float)(Count + 1) / (float)(Buckets); }
		float GetLoadFactorThrehsold() const { return LoadFactorThreshold; }
		uint64 GetCount() const { return Count; }
		uint64 GetBuckets() const { return Buckets; }
		bool IsEmpty() const { return Count == 0; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateBucket(Size);

			Data = (Node**)Memory::Allocate(sizeof(Node*) * Buckets, NEXUS_MEMORY_ALIGN, Allocator);
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Data[Index] = nullptr;
			}
		}

		Node* Allocate()
		{
			Count++;

			Node* New = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			New->Next = nullptr;
			return New;
		}

		void Free(Node** Pointer)
		{
			Memory::Free(Pointer, Allocator);
		}

		void Free(Node* Instance)
		{
			Count--;
			Memory::Free(Instance, Allocator);
		}

		void AppendNode(uint64 Index, Node* Instance)
		{
			if (Data[Index] != nullptr)
			{
				Instance->Next = Data[Index];
			}

			Data[Index] = Instance;
		}

		void RemoveNode(uint64 Index, Node* Instance)
		{
			Node* Current = Data[Index];
			while (Current->Next)
			{
				if (Current->Next == Instance)
				{
					Current->Next = Instance->Next;
					break;
				}

				Current = Current->Next;
			}

			if (Data[Index] == Instance)
			{
				Data[Index] = Instance->Next;
			}
		}

		uint64 GetIndex(const K& Key) const
		{
			uint64 Index = Hash<K, H>::HashObject(Key) % Buckets;
			return Index;
		}

		Node* GetNode(uint64 Index, const K& Key) const
		{
			Node* Current = Data[Index];
			while (Current)
			{
				if (Current->KeyValue.Key == Key)
				{
					break;
				}

				Current = Current->Next;
			}

			return Current;
		}

		Node* GetNode(const K& Key) const
		{
			uint64 Index = GetIndex(Key);
			return GetNode(Index, Key);
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Allocator = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
		}

		void ValidateBucket(uint64 Size)
		{
			Buckets = Size > 3 ? Size : 3;
		}

		inline static const uint64 DefaultSize = 16;
		inline static const float LoadFactorThreshold = LF;

		Allocator* Allocator;
		uint64 Buckets;
		uint64 Count;
		Node** Data;
	};
}