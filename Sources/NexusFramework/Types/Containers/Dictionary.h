#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Misc/Iterator.h"
#include "NexusFramework/Types/Containers/Misc/Node.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Misc/Hash/Hash.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename K, typename T, class H = Hashing::Default>
	class Dictionary
	{
	public:
		using KV = KeyValuePair<const K, T>;
		using N = Node::NodeHashmap<KV>;
		using I = Iterator::Hashmap<KV, N>;
		using TB = typename N::TombstoneMode;
		using Q = typename SimilarOf<K>::Type;

		inline static const uint64 DefaultSize = 11;

		Dictionary(uint64 Size = DefaultSize, Allocator * Allctr = Allocator::TryGet())
			: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Size);
		}

		Dictionary(InitializerList<KV> Init, Allocator* Allctr = Allocator::TryGet())
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
				Copy(Index, Other.Data[Index]);
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
				Copy(Index, Other.Data[Index]);
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
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.Value = Value;
			return Instance.Value.Value;
		}

		T& Assign(const K& Key, T&& Value)
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
			N& Instance = Data[Index];
			Instance.Value.Value = Move(Value);
			return Instance.Value.Value;
		}

		template<typename... Args>
		T& AssignConstruct(const K& Key, Args&&... args)
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
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
				NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
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
			NX_ASSERT(IsFreeIndex(Index), Default, "Key already present in Dictionary");
			Construct(Index, Hash, Key, Value);
			return Data[Index].Value.Value;
		}

		T& Append(K&& Key, T&& Value)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NX_ASSERT(IsFreeIndex(Index), Default, "Key already present in Dictionary");
			Construct(Index, Hash, Move(Key), Move(Value));
			return Data[Index].Value.Value;
		}

		template<typename... Args>
		T& AppendConstruct(const K& Key, Args&&... args)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NX_ASSERT(IsFreeIndex(Index), Default, "Key already present in Dictionary");
			Construct(Index, Hash, Key, args...);
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
				NX_ASSERT(IsFreeIndex(Index), Default, "Key already present in Dictionary");
				Construct(Index, Hash, It->Key, It->Value);
			}

			uint64 Hash = GetHash(Value.Begin()->Key);
			uint64 Index = GetIndex(Hash);
			return Data[Index].Value.Value;
		}

		T& TryAppend(const K& Key, const T& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].Free)
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Key, Value);
			}
			return Data[Index].Value.Value;
		}

		T& TryAppend(K&& Key, T&& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].Free)
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
		T& TryAppend(K&& Key, Args&&... args)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].Free)
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Move(Key), args...);
			}
			return Data[Index].Value.Value;
		}

		template<typename C>
		T& TryAppendRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->Key);
				uint64 Index = GetIndex(Hash);
				if (Index >= Capacity || Data[Index].Free)
				{
					if (Resize(++Count))
					{
						Index = GetIndex(Hash);
					}
					Construct(Index, Hash, It->Key, It->Value);
				}
			}

			uint64 Hash = GetHash(Value.Begin()->Key);
			uint64 Index = GetIndex(Hash);
			return Data[Index].Value.Value;
		}

		T& AppendOrAssign(const K& Key, const T& Value)
		{
			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index < Capacity && !Data[Index].Free)
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
			if (Index < Capacity && !Data[Index].Free)
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
			if (Index < Capacity && !Data[Index].Free)
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
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
			Destruct(Index);
			Resize(--Count);
		}

		template<typename C>
		void RemoveRange(const C& Value)
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(*It);
				uint64 Index = GetIndex(Hash);
				NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
				Destruct(Index);
				Resize(--Count);
			}
		}

		template<typename CK, typename CT>
		void RemoveRange(const Dictionary<CK, CT>& Value)
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			for (typename Dictionary<CK, CT>::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->Key);
				uint64 Index = GetIndex(Hash);
				NX_ASSERT(IsValidIndex(Index), Default, "Failed to find key");
				Destruct(Index);
				Resize(--Count);
			}
		}

		void TryRemove(const Q& Key)
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Key);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].Free)
			{
				return;
			}
			Destruct(Index);
			Resize(--Count);
		}

		template<typename C>
		void TryRemoveRange(const C& Value)
		{
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(*It);
				uint64 Index = GetIndex(Hash);
				if (Index >= Capacity || Data[Index].Free)
				{
					continue;
				}
				Destruct(Index);
				Resize(--Count);
			}
		}

		template<typename CK, typename CT>
		void TryRemoveRange(const Dictionary<CK, CT>& Value)
		{
			for (typename Dictionary<CK, CT>::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(It->Key);
				uint64 Index = GetIndex(Hash);
				if (Index >= Capacity || Data[Index].Free)
				{
					continue;
				}
				Destruct(Index);
				Resize(--Count);
			}
		}

		void Clear()
		{
			DestructRange(0, Capacity);
			Resize(0);
		}

		T& Get(const Q& Value)
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			KV* Item = TryGetItem(Value);
			NX_ASSERT(Item, Default, "Failed to find key");
			return Item->Value;
		}

		const T& Get(const Q& Value) const
		{
			NX_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			const KV* Item = TryGetItem(Value);
			NX_ASSERT(Item, Default, "Failed to find key");
			return Item->Value;
		}

		T* TryGet(const Q& Value)
		{
			KV* Item = TryGetItem(Value);
			return Item ? &Item->Value : nullptr;
		}

		const T* TryGet(const Q& Value) const
		{
			const KV* Item = TryGetItem(Value);
			return Item ? &Item->Value : nullptr;
		}

		I GetIterator(const Q& Key)
		{
			return TryGetIt(Key);
		}

		const I GetIterator(const Q& Key) const
		{
			return TryGetIt(Key);
		}

		I begin() { return Begin(); }
		I Begin()
		{
			return GetIt(0);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIt(0);
		}

		I end() { return End(); }
		I End()
		{
			return GetIt(Capacity);
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIt(Capacity);
		}

		void Reserve(uint64 Size)
		{
			Size = Math::Max(Size, Count);
			Reallocate(Size);
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
				Data[Index].Clear();
			}
		}

		void Reallocate(uint64 Size)
		{
			N* Temp = Data;
			uint64 Length = Capacity;

			Allocate(Size);

			for (uint64 OldIndex = 0; OldIndex < Length; ++OldIndex)
			{
				N& Instance = Temp[OldIndex];
				if (Instance.Free)
				{
					continue;
				}

				uint64 Hash = GetHash(Instance.Value.Key);
				uint64 NewIndex = GetIndex(Hash);
				Construct(NewIndex, Instance.Hash, Move(Instance.Value.Key), Move(Instance.Value.Value));
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
			NX_ASSERT(Instance.Free, Default, "Construct on an already occupied slot");
			Memory::Construct<KV>(&Instance.Value, args...);
			Instance.Update(Hash);
		}

		void Destruct(uint64 Index)
		{
			N& Instance = Data[Index];
			NX_ASSERT(!Instance.Free, Default, "Destruct on a free slot");
			Memory::Destruct(&Instance.Value);
			Instance.Update();
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				N& Instance = Data[Index + Offset];
				if (!Instance.Free)
				{
					Memory::Destruct(&Instance.Value);
					Instance.Update();
				}
			}
		}

		void Copy(uint64 Index, N& Instance)
		{
			if (!Instance.Free)
			{
				Construct(Index, Instance.Hash, Instance.Value);
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

				if (Instance.Free)
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

		KV& GetItem(uint64 Index)
		{
			return Data[Index].Value;
		}

		const KV& GetItem(uint64 Index) const
		{
			return Data[Index].Value;
		}

		KV* TryGetItem(const Q& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			return IsValidIndex(Index) ? &Data[Index].Value : nullptr;
		}

		const KV* TryGetItem(const Q& Value) const
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			return IsValidIndex(Index) ? &Data[Index].Value : nullptr;
		}

		I GetIt(uint64 Index)
		{
			return I(Data, Index, Capacity);
		}

		const I GetIt(uint64 Index) const
		{
			return I(Data, Index, Capacity);
		}

		I TryGetIt(const Q& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			return IsValidIndex(Index) ? GetIt(Index) : End();
		}

		const I TryGetIt(const Q& Value) const
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			return IsValidIndex(Index) ? GetIt(Index) : End();
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

		bool IsValidIndex(uint64 Index) const
		{
			return Index < Capacity && !Data[Index].Free;
		}

		bool IsFreeIndex(uint64 Index) const
		{
			return Index < Capacity && Data[Index].Free;
		}

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		N* Data;
	};
}
