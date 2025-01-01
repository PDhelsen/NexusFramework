#pragma once

#include "Types/Numbers/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/AllocatorContext.h"
#include "Misc/Templates.h"
#include "Misc/Hash.h"
#include "Misc/Math.h"
#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Debug/Logger/Log.h"

namespace NxEn
{
	template<typename K, typename T, class H = Hashing::Default>
	class Dictionary
	{
	public:
		using KV = KeyValuePair<const K, T>;
		using N = Node::NodeHashmap<KV>;
		using I = Iterator::IteratorHashmap<KV, N>;

		inline static const uint64 DefaultSize = 11;

		Dictionary(uint64 Size = DefaultSize, Allocator * Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Size);
		}

		Dictionary(const Dictionary<K, T, H>& Other)
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			Allocate(Capacity);

			for (uint64 Index = 0; Index < Capacity; ++Index)
			{
				N& Instance = Other.Data[Index];
				if (!Instance.IsFree())
				{
					Construct(Index, Instance.Hash, Instance.Value);
				}
			}
		}

		Dictionary(Dictionary<K, T, H>&& Other) noexcept
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Capacity = 0;
			Other.Count = 0;
			Other.Data = nullptr;
		}

		~Dictionary()
		{
			Clear();
			Free();
		}

		Dictionary<K, T, H>& operator=(const Dictionary<K, T, H>& Other)
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Capacity = Other.Capacity;
			Count = Other.Count;

			Allocate(Capacity);

			for (uint64 Index = 0; Index < Capacity; ++Index)
			{
				N& Instance = Other.Data[Index];
				if (!Instance.IsFree())
				{
					Construct(Index, Instance.Hash, Instance.Value);
				}
			}

			return *this;
		}

		Dictionary<K, T, H>& operator=(Dictionary<K, T, H>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Alloc = Other.Alloc;
			Capacity = Other.Capacity;
			Count = Other.Count;
			Data = Other.Data;

			Other.Capacity = 0;
			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
		}

		T& operator[](const K& Key)
		{
			return Get(Key);
		}

		const T& operator[](const K& Key) const
		{
			return Get(Key);
		}

		bool operator==(const Dictionary<K, T, H>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Dictionary<K, T, H>& Other) const
		{
			return !(*this == Other);
		}

		T& Assign(const K& Key, const T& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, Default, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.Value = Value;
			return Instance.Value.Value;
		}

		T& Assign(const K& Key, T&& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, Default, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.Value = Move(Value);
			return Instance.Value.Value;
		}

		template<typename... Args>
		T& AssignConstruct(const K& Key, Args&&... args)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, Default, "Failed to find key");
			N& Instance = Data[Index];
			Memory::Construct<T>(&Instance.Value.Value, args...);
			return Instance.Value.Value;
		}

		template<typename C>
		T& AssignRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->Key);
				uint64 Index = GetIndexRead(Hash);
				NEXUS_ASSERT(Index < Capacity, Default, "Failed to find key");
				GetItem(Index).Value = It->Value;
			}

			uint64 Hash = GetHash(Value.Begin()->Key);
			uint64 Index = GetIndexRead(Hash);
			return GetItem(Index).Value;
		}

		T& Append(const K& Key, const T& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return GetItem(Index).Value;
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Key, Value);
			return GetItem(Index).Value;
		}

		T& Append(K&& Key, T&& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return GetItem(Index).Value;
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Move(Key), Move(Value));
			return GetItem(Index).Value;
		}

		template<typename... Args>
		T& AppendConstruct(K&& Key, Args&&... args)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return GetItem(Index).Value;
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Move(Key), args...);
			return GetItem(Index).Value;
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			Resize(GetCount() + Value.GetCount());

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->Key);
				uint64 Index = GetIndexRead(Hash);
				if (Index != Capacity)
				{
					continue;
				}

				Index = GetIndexWrite(Hash);
				Construct(Index, Hash, It->Key, It->Value);
			}

			uint64 Hash = GetHash(Value.Begin()->Key);
			uint64 Index = GetIndexRead(Hash);
			return GetItem(Index).Value;
		}

		void Remove(const K& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index != Capacity, Default, "Failed to find key");
			Destruct(Index);
			Resize(--Count);
		}

		void Clear()
		{
			DestructRange(0, Capacity);
			Resize(0);
		}

		T& Get(const K& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, Default, "Failed to find key");
			return GetItem(Index).Value;
		}

		const T& Get(const K& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, Default, "Failed to find key");
			return GetItem(Index).Value;
		}

		T* TryGet(const K& Key) 
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index >= Capacity)
			{
				return nullptr;
			}
			return &GetItem(Index).Value;
		}

		const T* TryGet(const K& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index >= Capacity)
			{
				return nullptr;
			}
			return &GetItem(Index).Value;
		}

		I GetIterator(const K& Key)
		{
			return GetIteratorKey(Key);
		}

		const I GetIterator(const K& Key) const
		{
			return GetIteratorKey(Key);
		}

		I begin() { return Begin(); }
		I Begin()
		{
			return GetIteratorIndex(0);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIteratorIndex(0);
		}

		I end() { return End(); }
		I End()
		{
			return GetIteratorIndex(Capacity);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIteratorIndex(Capacity);
		}

		void Grow(uint64 Size)
		{
			if (Size <= Capacity)
			{
				return;
			}

			Reallocate(Size);
		}

		void Shrink(uint64 Size = 0)
		{
			if (Size > Capacity)
			{
				return;
			}

			Reallocate(Size);
		}

		void Swap(const K& A, const K& B)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			T Temp = Get(A);
			Get(A) = Move(Get(B));
			Get(B) = Move(Temp);
		}

		bool ContainsKey(const K& Key) const
		{
			return GetIteratorKey(Key) != End();
		}

		bool ContainsValue(const T& Value) const
		{
			return GetIteratorValue(Value) != End();
		}

		I FindKey(const K& Key) 
		{
			return GetIteratorKey(Key);
		}

		const I FindKey(const K& Key) const
		{
			return GetIteratorKey(Key);
		}

		I FindValue(const T& Value) 
		{
			return GetIteratorValue(Value);
		}

		const I FindValue(const T& Value) const
		{
			return GetIteratorValue(Value);
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (N*)Memory::Allocate(sizeof(N) * Capacity, Alloc);

			for (uint64 Index = 0; Index < Capacity; ++Index)
			{
				Data[Index].Hash = 0;
			}
		}

		void Reallocate(uint64 Size)
		{
			N* Temp = Data;
			uint64 Length = Capacity;

			Allocate(Size);

			for (uint64 OldIndex = 0; OldIndex < Length; ++OldIndex)
			{
				N& Old = Temp[OldIndex];
				if (Old.IsFree())
				{
					continue;
				}

				uint64 Hash = GetHash(Old.Value.Key);
				uint64 NewIndex = GetIndexWrite(Hash);
				Construct(NewIndex, Hash, Move(Old.Value.Key), Move(Old.Value.Value));
			}

			Memory::Free(Temp, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
			Data = nullptr;
		}

		template<typename... Args>
		void Construct(uint64 Index, uint64 Hash, Args&&... args)
		{
			N& Instance = Data[Index];
			NEXUS_ASSERT(Instance.IsFree(), Default, "Construct on an already occupied slot");
			Memory::Construct<KV>(&Instance.Value, args...);
			Instance.Hash = Hash;
		}

		void Destruct(uint64 Index)
		{
			N& Instance = Data[Index];
			NEXUS_ASSERT(!Instance.IsFree(), Default, "Destruct on a free slot");
			Memory::Destruct(&Instance.Value);
			Instance.Hash = 0;
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				N& Instance = Data[Index + Offset];
				if (!Instance.IsFree())
				{
					Memory::Destruct(&Instance.Value);
					Instance.Hash = 0;
				}
			}
		}

		uint64 GetHash(const K& Value) const
		{
			return Hash<H>::HashObject(Value);
		}

		uint64 GetIndexHash(uint64 Hash) const
		{
			return Hash % Capacity;
		}

		uint64 GetIndexWrite(uint64 Hash) const
		{
			uint64 IndexHashed = GetIndexHash(Hash);

			uint64 Iteration = 0;
			uint64 Index = IndexHashed;

			while (!Data[Index].IsFree())
			{
				Index = ProbingPolicy(IndexHashed, ++Iteration);
				if (Iteration >= MaxProbingIteration())
				{
					NEXUS_ASSERT(false, Default, "Failed to find a free spot");
					return Capacity;
				}
			}

			return Index;
		}

		uint64 GetIndexRead(uint64 Hash) const
		{
			uint64 IndexHashed = GetIndexHash(Hash);

			uint64 Iteration = 0;
			uint64 Index = IndexHashed;

			while (Data[Index].IsFree() || Data[Index].Hash != Hash)
			{
				Index = ProbingPolicy(IndexHashed, ++Iteration);
				if (Iteration >= MaxProbingIteration())
				{
					return Capacity;
				}
			}

			return Index;
		}

		KV& GetItem(uint64 Index) const
		{
			return Data[Index].Value;
		}

		I GetIteratorIndex(uint64 Index) const
		{
			return I(Data, Index, Capacity);
		}

		I GetIteratorKey(const K& Key) const
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			return Index != Capacity ? I(Data, Index, Capacity) : End();
		}

		I GetIteratorValue(const T& Value) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (It->Value == Value)
				{
					return It;
				}
			}

			return End();
		}

		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				Reallocate(GrowPolicy());
			}
		}

		void ValidateCapacity(uint64 Size)
		{
			Capacity = Size > 3 ? Size : 3;
		}

		uint64 GrowPolicy() const
		{
			return Math::NextPrime(Capacity);
		}

		uint64 ProbingPolicy(uint64 Index, uint64 Iteration) const
		{
			if (Iteration < Capacity)
			{
				bool Flip = Iteration % 2 == 0;
				uint64 Offset = Math::CeilToInt((double)(Iteration) / 2.0);
				Index = Index + (Flip ? -1 : 1) * (Offset * Offset);
				Index = Math::Modulo(Index, Capacity);
				return Index;
			}
			else
			{
				return Iteration - Capacity;
			}
		}

		uint64 MaxProbingIteration() const
		{
			return Capacity * 2;
		}

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		N* Data;
	};
}
