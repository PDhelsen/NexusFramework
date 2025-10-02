#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Types/Containers/Node.h"
#include "NexusFramework/Types/Containers/Iterator.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename K, typename T, class H = Hashing::Default>
	class Dictionary
	{
	public:
		using KV = KeyValuePair<const K, T>;
		using N = Node::NodeHashmap<KV>;
		using I = Iterator::IteratorHashmap<KV, N>;
		using TB = typename N::TombstoneMode;
		using Q = typename SimilarOf<K>::Type;

		inline static const uint64 DefaultSize = 11;

		Dictionary(uint64 Size = DefaultSize, Allocator * Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Size);
		}

		Dictionary(InitializerList<KV> Init, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Init.size());

			for (auto& It : Init)
			{
				Append(It.Key, It.Value);
			}
		}

		Dictionary(const Dictionary<K, T, H>& Other)
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			Allocate(Capacity);

			for (uint64 Index = 0; Index < Capacity; ++Index)
			{
				Copy(Index, Other.Data[Index], false);
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
			if (this == &Other)
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
				Copy(Index, Other.Data[Index], false);
			}

			return *this;
		}

		Dictionary<K, T, H>& operator=(Dictionary<K, T, H>&& Other) noexcept
		{
			if (this == &Other)
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

		T& operator[](const Q& Key)
		{
			return Get(Key);
		}

		const T& operator[](const Q& Key) const
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

		bool operator>(const Dictionary<K, T, H>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Dictionary<K, T, H>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Dictionary<K, T, H>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Dictionary<K, T, H>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Assign(const K& Key, const T& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.Value = Value;
			return Instance.Value.Value;
		}

		T& Assign(const K& Key, T&& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.Value = Move(Value);
			return Instance.Value.Value;
		}

		template<typename... Args>
		T& AssignConstruct(const K& Key, Args&&... args)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
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
				uint64 Index = GetIndex(Hash);
				NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
				N& Instance = Data[Index];
				Instance.Value.Value = Value;
			}

			uint64 Hash = GetHash(Value.Begin()->Key);
			uint64 Index = GetIndexRead(Hash);
			return Data[Index].Value.Value;
		}

		T& Append(const K& Key, const T& Value)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Key already present in Dictionary");
			Construct(Index, Hash, Key, Value);
			return Data[Index].Value.Value;
		}

		T& Append(K&& Key, T&& Value)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Key already present in Dictionary");
			Construct(Index, Hash, Move(Key), Move(Value));
			return Data[Index].Value.Value;
		}

		template<typename... Args>
		T& AppendConstruct(K&& Key, Args&&... args)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Key already present in Dictionary");
			Construct(Index, Hash, Move(Key), args...);
			return Data[Index].Value.Value;
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			Resize(GetCount() + Value.GetCount());

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->Key);
				uint64 Index = GetIndex(Hash);
				NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Key already present in Dictionary");
				Construct(Index, Hash, It->Key, It->Value);
			}

			uint64 Hash = GetHash(Value.Begin()->Key);
			uint64 Index = GetIndex(Hash);
			return Data[Index].Value.Value;
		}

		T& AppendOrAssign(const K& Key, const T& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			uint64 SaveIndex = Index;
			if (Index < Capacity && !Data[Index].IsFree())
			{
				N& Instance = Data[Index];
				Instance.Value.Value = Value;
			}
			else
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Key, Value);
			}
			return Data[Index].Value.Value;
		}

		T& AppendOrAssign(K&& Key, T&& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index < Capacity && !Data[Index].IsFree())
			{
				N& Instance = Data[Index];
				Instance.Value.Value = Move(Value);
			}
			else
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Move(Key), Move(Value));
			}
			return Data[Index].Value.Value;
		}

		template<typename... Args>
		T& AppendOrAssignConstruct(K&& Key, Args&&... args)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index < Capacity && !Data[Index].IsFree())
			{
				N& Instance = Data[Index];
				Memory::Construct<T>(&Instance.Value.Value, args...);
			}
			else
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Move(Key), args...);
			}
			return Data[Index].Value.Value;
		}

		void Remove(const Q& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
			Destruct(Index);
			Resize(--Count);
		}

		void Clear()
		{
			DestructRange(0, Capacity);
			Resize(0);
		}

		T& Get(const Q& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
			return Data[Index].Value.Value;
		}

		const T& Get(const Q& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
			return Data[Index].Value.Value;
		}

		T* TryGet(const Q& Key)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			return Index < Capacity && !Data[Index].IsFree() ? &Data[Index].Value.Value : nullptr;
		}

		const T* TryGet(const Q& Key) const
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			return Index < Capacity && !Data[Index].IsFree() ? &Data[Index].Value.Value : nullptr;
		}

		I GetIterator(const Q& Key)
		{
			return GetIteratorKey(Key);
		}

		const I GetIterator(const Q& Key) const
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

		bool ContainsKey(const Q& Key) const
		{
			return GetIteratorKey(Key) != End();
		}

		bool ContainsValue(const T& Value) const
		{
			return GetIteratorValue(Value) != End();
		}

		I FindKey(const Q& Key)
		{
			return GetIteratorKey(Key);
		}

		const I FindKey(const Q& Key) const
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
				Data[Index].Tombstone = TB::NotTombstone;
			}
		}

		void Reallocate(uint64 Size)
		{
			N* Temp = Data;
			uint64 Length = Capacity;

			Allocate(Size);

			for (uint64 OldIndex = 0; OldIndex < Length; ++OldIndex)
			{
				if (Temp[OldIndex].IsFree() && Temp[OldIndex].Tombstone == TB::NotTombstone)
				{
					continue;
				}

				uint64 Hash = GetHash(Temp[OldIndex].Value.Key);
				uint64 NewIndex = GetIndex(Hash);
				Copy(NewIndex, Temp[OldIndex], true);
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
			if (Data[Index].Tombstone == TB::IsTombstone)
			{
				Data[Index].Tombstone = TB::WasTombstone;
			}
		}

		void Destruct(uint64 Index)
		{
			N& Instance = Data[Index];
			NEXUS_ASSERT(!Instance.IsFree(), Default, "Destruct on a free slot");
			Memory::Destruct(&Instance.Value);
			Instance.Hash = 0;
			Instance.Tombstone = TB::IsTombstone;
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
					Instance.Tombstone = TB::IsTombstone;
				}
			}
		}

		void Copy(uint64 Index, N& Instance, bool MoveData)
		{
			if (!Instance.IsFree())
			{
				if (MoveData)
				{
					Construct(Index, Instance.Hash, Move(Instance.Value.Key), Move(Instance.Value.Value));
				}
				else
				{
					Construct(Index, Instance.Hash, Instance.Value);
				}
			}

			Data[Index].Tombstone = Instance.Tombstone;
		}

		uint64 GetHash(const Q& Value) const
		{
			return Hash<H>::HashObject(Value);
		}

		uint64 GetIndex(uint64 Hash) const
		{
			uint64 H1 = Hash % Capacity;
			uint64 H2 = 1 + (Hash % (Capacity - 1));

			uint64 Index = H1;
			uint64 Tombstone = Capacity;

			for (uint64 Iteration = 0; Iteration < Capacity; ++Iteration)
			{
				N& Instance = Data[Index];

				if (Instance.IsFree())
				{
					if (Instance.Tombstone == TB::NotTombstone)
					{
						return (Tombstone != Capacity) ? Tombstone : Index;
					}
					else
					{
						if (Tombstone == Capacity)
						{
							Tombstone = Index;
						}
					}
				}
				else if (Instance.Hash == Hash)
				{
					return Index;
				}

				Index = (Index + H2) % Capacity;
			}

			return Tombstone != Capacity ? Tombstone : Capacity;
		}

		KV& GetItem(uint64 Index) const
		{
			return Data[Index].Value;
		}

		I GetIteratorIndex(uint64 Index) const
		{
			return I(Data, Index, Capacity);
		}

		I GetIteratorKey(const Q& Key) const
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			return Index < Capacity && !Data[Index].IsFree() ? I(Data, Index, Capacity) : End();
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

		bool Resize(uint64 Size)
		{
			Count = Size;

			if (Count > Capacity)
			{
				Reallocate(GrowPolicy());
				return true;
			}

			return false;
		}

		void ValidateCapacity(uint64 Size)
		{
			Capacity = Size > 3 ? Size : 3;
			Capacity = Math::IsPrime(Capacity) ? Capacity : Math::NextPrime(Capacity);
		}

		uint64 GrowPolicy() const
		{
			return Capacity * 2;
		}

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		N* Data;
	};
}
