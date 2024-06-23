#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"
#include "Misc/Hash.h"

namespace NxEn
{
	// TODO: Implementation - Containers - Union / Intersect / Exclude
	template<typename K, typename T, class H = Fnv1a64, float LF = 1.0f>
	class Dictionary
	{
	public:
		using Node = LinkedNodeSimple<KeyValuePair<K, T>>;
		using Iterator = HashmapIterator<KeyValuePair<K, T>, Node>;

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
					NodeCopy->Value.Initialize(Current->Value.GetKey(), Current->Value.GetValue());
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

		T& Assign(const K& Key, const T& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->Value.SetValue(Value);

			return Instance->Value.GetValue();
		}

		T& Assign(const K& Key, T&& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->Value.SetValue(Move(Value));
			
			return Instance->Value.GetValue();
		}

		template<typename... Args>
		T& AssignConstruct(const K& Key, Args&&... args)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* Instance = GetNode(Key);
			NEXUS_ASSERT(Instance, "Key not in the Dictionary");
			Instance->Value.SetValue(args...);
		
			return Instance->Value.GetValue();
		}

		template<typename C>
		T& AssignRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				Assign(It->GetKey(), It->GetValue());
			}

			return GetNode(Value.Begin()->GetKey())->Value.GetValue();
		}

		T& Append(const K& Key, const T& Value)
		{
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);
			if (Instance != nullptr)
			{
				return Instance->Value.GetValue();
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}

			Instance = Allocate();
			Instance->Value.Initialize(Key, Value);

			AppendNode(Index, Instance);
			return Instance->Value.GetValue();
		}

		T& Append(const K& Key, T&& Value)
		{
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);
			if (Instance != nullptr)
			{
				return Instance->Value.GetValue();
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}

			Instance = Allocate();
			Instance->Value.Initialize(Key, Move(Value));

			AppendNode(Index, Instance);
			return Instance->Value.GetValue();
		}

		template<typename... Args>
		T& AppendConstruct(const K& Key, Args&&... args)
		{
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);
			if (Instance != nullptr)
			{
				return Instance->Value.GetValue();
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}

			Instance = Allocate();
			Instance->Value.Initialize(Key, args...);

			AppendNode(Index, Instance);
			return Instance->Value.GetValue();
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				Append(It->GetKey(), It->GetValue());
			}

			return GetNode(Value.Begin()->GetKey())->Value.GetValue();
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
			return Instance->Value.GetValue();
		}

		T* TryGet(const K& Key) const
		{
			Node* Instance = GetNode(Key);
			if (!Instance)
			{
				return nullptr;
			}
			return &Instance->Value.GetValue();
		}

		Iterator GetIterator(const K& Key)
		{
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);
			if (!Instance)
			{
				return End();
			}
			return Iterator(Data, Instance, Buckets, Index);
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, Data[0], Buckets, 0);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data, nullptr, Buckets, Buckets);
		}

		void ReHash(uint64 Size)
		{
			Resize(Size);
		}

		void Swap(const K& A, const K& B)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");
			
			Node* NodeA = GetNode(A);
			Node* NodeB = GetNode(B);

			NEXUS_ASSERT(NodeA, "Key A not in the Dictionary");
			NEXUS_ASSERT(NodeB, "Key B not in the Dictionary");

			T Temp = NodeA->Value.GetValue();
			NodeA->Value.GetValue() = Move(NodeB->Value.GetValue());
			NodeB->Value.GetValue() = Move(Temp);
		}

		bool ContainsKey(const K& Key) const
		{
			return FindKey(Key) != End();
		}

		bool ContainsValue(const T& Other) const
		{
			return FindValue(Other) != End();
		}

		Iterator FindKey(const K& Key) const
		{
			uint64 Index = GetIndex(Key);
			Node* Instance = GetNode(Index, Key);
			if (Instance)
			{
				return Iterator(Data, Instance, Buckets, Index);
			}

			return End();
		}

		Iterator FindValue(const T& Other) const
		{
			for (Iterator It = Begin(); It != End(); It++)
			{
				if (It->GetValue() == Other)
				{
					return It;
				}
			}

			return End();
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetBuckets() const { return Buckets; }
		float GetLoadFactor() const { return (float)(Count + 1) / (float)(Buckets); }
		float GetLoadFactorThrehsold() const { return LoadFactorThreshold; }

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

			Memory::Destruct(&Instance->Value);
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
				if (Current->Value.GetKey() == Key)
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
					uint64 Index = GetIndex(Current->Value.GetKey());
					AppendNode(Index, Current);

					Current = Next;
				}
			}

			Free(TempArray);
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
