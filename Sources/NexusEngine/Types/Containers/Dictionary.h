#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"
#include "Misc/Hash.h"
#include "Misc/Math.h"

namespace NxEn
{
	template<typename K, typename T, class H = Hashing::Default>
	class Dictionary
	{
	public:
		using N = Node::NodeHashmap<KeyValuePair<K, T>>;
		using I = Iterator::IteratorHashmap<KeyValuePair<K, T>, N>;

		Dictionary(uint64 Size = DefaultSize, Allocator* Allctr = nullptr)
			: Alloc(nullptr), Capacity(0), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
		}

		Dictionary(const Dictionary<K, T, H>& Other)
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Dictionary - Copy constructor");

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
			Other.Data = nullptr;
		}

		~Dictionary()
		{
			Clear();
			Free();
		}

		Dictionary<K, T, H>& operator=(const Dictionary<K, T, H>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Dictionary - Assignement operator");

			if (*this == Other)
			{
				return *this;
			}

			Clear();
			Free();

			Alloc = Other.Alloc;
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

		T& operator[](const K& Key) const
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
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.SetValue(Value);
			return Instance.Value.GetValue();
		}

		T& Assign(const K& Key, T&& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.SetValue(Move(Value));
			return Instance.Value.GetValue();
		}

		template<typename... Args>
		T& AssignConstruct(const K& Key, Args&&... args)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.SetValueConstruct(args...);
			return Instance.Value.GetValue();
		}

		template<typename C>
		T& AssignRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->GetKey());
				uint64 Index = GetIndexRead(Hash);
				NEXUS_ASSERT(Index < Capacity, "Failed to find key");
				Data[Index].Value->SetValue(It->GetValue());
			}

			uint64 Hash = GetHash(Value.Begin()->GetKey());
			uint64 Index = GetIndexRead(Hash);
			return Data[Index].Value.GetValue();
		}

		T& Append(const K& Key, const T& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return Data[Index].Value.GetValue();
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Key, Value);
			return Data[Index].Value.GetValue();
		}

		T& Append(K&& Key, T&& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return Data[Index].Value.GetValue();
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Move(Key), Move(Value));
			return Data[Index].Value.GetValue();
		}

		template<typename... Args>
		T& AppendConstruct(K&& Key, Args&&... args)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return Data[Index].Value.GetValue();
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Move(Key), args...);
			return Data[Index].Value.GetValue();
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			Resize(GetCount() + Value.GetCount());

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->GetKey());
				uint64 Index = GetIndexRead(Hash);
				if (Index != Capacity)
				{
					continue;
				}

				Index = GetIndexWrite(Hash);
				Construct(Index, Hash, It->GetKey(), It->GetValue());
			}

			uint64 Hash = GetHash(Value.Begin()->GetKey());
			uint64 Index = GetIndexRead(Hash);
			return Data[Index].Value.GetValue();
		}

		void Remove(const K& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index != Capacity, "Failed to find key");
			Destruct(Index);
			Resize(--Count);
		}

		void Clear()
		{
			DestructRange(0, Capacity);
			Resize(0);
		}

		T& Get(const K& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			return Data[Index].Value.GetValue();
		}

		T* TryGet(const K& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			if (Index >= Capacity)
			{
				return nullptr;
			}
			return &Data[Index].Value.GetValue();
		}

		I GetIterator(const K& Key)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			return Index != Capacity ? I(Data, Index, Capacity) : End();
		}

		I begin() const { return Begin(); }
		I Begin() const
		{
			return I(Data, 0, Capacity);
		}

		I end() const { return End(); }
		I End() const
		{
			return I(Data, Capacity, Capacity);
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
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			T Temp = Get(A);
			Get(A) = Move(Get(B));
			Get(B) = Move(Temp);
		}

		bool ContainsKey(const K& Key) const
		{
			return FindKey(Key) != End();
		}

		bool ContainsValue(const T& Value) const
		{
			return FindValue(Value) != End();
		}

		I FindKey(const K& Key) const
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndexRead(Hash);
			return Index != Capacity ? I(Data, Index, Capacity) : End();
		}

		I FindValue(const T& Value) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (It->GetValue() == Value)
				{
					return It;
				}
			}

			return End();
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
			NEXUS_LOG(Engine, Warning, "Performance", "Dictionary - Reallocate");

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

				uint64 Hash = GetHash(Old.Value.GetKey());
				uint64 NewIndex = GetIndexWrite(Hash);
				N& New = Data[NewIndex];

				New.Value = Move(Old.Value);
				New.Hash = Hash;
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
			NEXUS_ASSERT(Instance.IsFree(), "Construct on an already occupied slot");
			Memory::Construct<KeyValuePair<K, T>>(&Instance.Value, args...);
			Instance.Hash = Hash;
		}

		void Destruct(uint64 Index)
		{
			N& Instance = Data[Index];
			NEXUS_ASSERT(!Instance.IsFree(), "Destruct on a free slot");
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
				NEXUS_ASSERT(Iteration < Capacity, "Failed to find a free spot");

				NEXUS_LOG(Engine, Warning, "Performance", "Dictionary - Collision");
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
				if (Iteration >= Capacity)
				{
					return Capacity;
				}
			}

			return Index;
		}

		void Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				Reallocate(GrowPolicy());
			}
		}

		void ValidateAllocator(Allocator* Allctr)
		{
			Alloc = Allctr != nullptr ? Allctr : Memory::GetActiveAllocator();
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
			bool Flip = Iteration % 2 == 0;
			uint64 Offset = Math::CeilToInt((double)(Iteration) / 2.0);
			Index = Index + (Flip ? -1 : 1) * (Offset * Offset);
			Index = Math::Modulo(Index, Capacity);
			return Index;
		}

		inline static const uint64 DefaultSize = 11;

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		N* Data;
	};
}
