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
	template<typename T, class H = Fnv1a64, float LF = 1.0f>
	class Set
	{
	public:
		using Node = LinkedNodeSimple<T>;
		using Iterator = HashmapIterator<const T, Node>;

		Set(uint64 Size = DefaultSize, Allocator* Allctr = nullptr)
			: Allocator(nullptr), Buckets(0), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
		}

		Set(const Set<T, H, LF>& Other)
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), Data(Other.Data)
		{
		}

		Set(Set<T, H, LF>&& Other) noexcept
			: Allocator(Other.Allocator), Buckets(Other.Buckets), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Set()
		{
			Clear();
			Free(Data);
		}

		Set<T, H, LF> Copy() const
		{
			Set<T, H, LF> Copy = Set<T, H, LF>(Buckets, Allocator);
			
			for (uint64 Index = 0; Index < Buckets; Index++)
			{
				Node* Current = Data[Index];
				while (Current)
				{
					Node* NodeCopy = Copy.Allocate();
					NodeCopy->Value = Current->Value;
					Copy.AppendNode(Index, NodeCopy);

					Current = Current->Next;
				}
			}

			return Copy;
		}

		bool operator==(const Set<T, H, LF>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Set<T, H, LF>& Other) const
		{
			return !(*this == Other);
		}

		const T& Append(const T& Value)
		{
			uint64 Index = GetIndex(Value);
			Node* Instance = GetNode(Index, Value);
			if (Instance != nullptr)
			{
				return Instance->Value;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}
			
			Instance = Allocate();
			Instance->Value = Value;

			AppendNode(Index, Instance);
			return Instance->Value;
		}

		const T& Append(T&& Value)
		{
			uint64 Index = GetIndex(Value);
			Node* Instance = GetNode(Index, Value);
			if (Instance != nullptr)
			{
				return Instance->Value;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}

			Instance = Allocate();
			Instance->Value = Move(Value);

			AppendNode(Index, Instance);
			return Instance->Value;
		}

		template<typename C>
		const T& AppendRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				Append(*It);
			}
			
			return GetNode(*(Value.Begin()))->Value;
		}

		void Remove(const T& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Set is empty");

			uint64 Index = GetIndex(Value);
			Node* Instance = GetNode(Index, Value);

			NEXUS_ASSERT(Instance, "Value not in Set");

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

		Iterator GetIterator(const T& Value)
		{
			uint64 Index = GetIndex(Value);
			Node* Instance = GetNode(Index, Value);
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

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		Iterator Find(const T& Other) const
		{
			uint64 Index = GetIndex(Other);
			Node* Instance = GetNode(Index, Other);
			if (Instance)
			{
				return Iterator(Data, Instance, Buckets, Index);
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

			Node* Instance = (Node*)Memory::Allocate(sizeof(Node), NEXUS_MEMORY_ALIGN, Allocator);
			Instance->Next = nullptr;
			return Instance;
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

			if (Instance == Data[Index])
			{
				Data[Index] = Instance->Next;
			}
		}

		uint64 GetIndex(const T& Value) const
		{
			uint64 Index = Hash<T, H>::HashObject(Value) % Buckets;
			return Index;
		}

		Node* GetNode(uint64 Index, const T& Value) const
		{
			Node* Current = Data[Index];
			while (Current)
			{
				if (Current->Value == Value)
				{
					break;
				}

				Current = Current->Next;
			}

			return Current;
		}

		Node* GetNode(const T& Value) const
		{
			uint64 Index = GetIndex(Value);
			return GetNode(Index, Value);
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
					uint64 Index = GetIndex(Current->Value);
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