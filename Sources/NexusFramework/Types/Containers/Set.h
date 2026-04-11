#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Types/Containers/Iterator.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename T, class H = Hashing::Default>
	class Set
	{
		friend class ContainersUtils;

	public:
		using N = Node::NodeHashmap<T>;
		using I = Iterator::IteratorHashmap<const T, N>;
		using TB = typename N::TombstoneMode;
		using Q = typename SimilarOf<T>::Type;

		inline static const uint64 DefaultSize = 11;

		Set(uint64 Size = DefaultSize, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Size);
		}

		Set(InitializerList<T> Init, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr)
		{
			Allocate(Init.size());

			for (auto& It : Init)
			{
				Append(It);
			}
		}

		Set(const Set<T, H>& Other)
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			Allocate(Capacity);

			for (uint64 Index = 0; Index < Capacity; ++Index)
			{
				Copy(Index, Other.Data[Index]);
			}
		}

		Set(Set<T, H>&& Other) noexcept
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Capacity = 0;
			Other.Count = 0;
			Other.Data = nullptr;
		}

		~Set()
		{
			Clear();
			Free();
		}

		Set<T, H>& operator=(const Set<T, H>& Other)
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

		Set<T, H>& operator=(Set<T, H>&& Other) noexcept
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

		bool operator==(const Set<T, H>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Set<T, H>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Set<T, H>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Set<T, H>& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Set<T, H>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Set<T, H>& Other) const
		{
			return Count <= Other.Count;
		}

		const T& Append(const T& Value)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Value already present in Dictionary");
			Construct(Index, Hash, Value);
			return Data[Index].Value;
		}

		const T& Append(T&& Value)
		{
			Resize(++Count);
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Value already present in Dictionary");
			Construct(Index, Hash, Move(Value));
			return Data[Index].Value;
		}

		template<typename C>
		const T& AppendRange(const C& Value)
		{
			Resize(GetCount() + Value.GetCount());

			for (typename C::I It = Value.Begin(); It != Value.End(); ++It)
			{
				uint64 Hash = GetHash(*It);
				uint64 Index = GetIndex(Hash);
				NEXUS_ASSERT(Index < Capacity && Data[Index].IsFree(), Default, "Value already present in Dictionary");
				Construct(Index, Hash, *It);
			}

			uint64 Hash = GetHash(*Value.Begin());
			uint64 Index = GetIndex(Hash);
			return Data[Index].Value;
		}

		const T& TryAppend(const T& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].IsFree())
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Value);
			}
			return Data[Index].Value;
		}

		const T& TryAppend(T&& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].IsFree())
			{
				if (Resize(++Count))
				{
					Index = GetIndex(Hash);
				}
				Construct(Index, Hash, Move(Value));
			}
			return Data[Index].Value;
		}

		void Remove(const Q& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			NEXUS_ASSERT(Index < Capacity && !Data[Index].IsFree(), Default, "Failed to find key");
			Destruct(Index);
			Resize(--Count);
		}

		void TryRemove(const Q& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), Default, "Dictionary is empty");

			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			if (Index >= Capacity || Data[Index].IsFree())
			{
				return;
			}
			Destruct(Index);
			Resize(--Count);
		}

		void Clear()
		{
			DestructRange(0, Capacity);
			Resize(0);
		}

		I GetIterator(const Q& Value)
		{
			return GetIteratorValue(Value);
		}

		const I GetIterator(const Q& Value) const
		{
			return GetIteratorValue(Value);
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

		void Reserve(uint64 Size)
		{
			Size = Math::Max(Size, Count);
			Reallocate(Size);
		}

		bool Contains(const T& Other) const { return GetIteratorValue(Other) != End(); }
		bool Contains(const Iterator::IteratorPredicate<T>& Predicate) const
		{
			return GetIteratorValue(Predicate) != End();
		}

		const I Find(const T& Other) const { return GetIteratorValue(Other); }
		const I Find(const Iterator::IteratorPredicate<T>& Predicate) const
		{
			return GetIteratorValue(Predicate);
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
				N& Instance = Temp[OldIndex];
				if (Instance.IsFree())
				{
					continue;
				}

				uint64 Hash = GetHash(Instance.Value);
				uint64 NewIndex = GetIndex(Hash);
				Construct(NewIndex, Instance.Hash, Move(Instance.Value));
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
			Memory::Construct<T>(&Instance.Value, args...);
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

		void Copy(uint64 Index, N& Instance)
		{
			if (!Instance.IsFree())
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

		T& GetItem(uint64 Index) const
		{
			return Data[Index].Value;
		}

		I GetIteratorIndex(uint64 Index) const
		{
			return I(Data, Index, Capacity);
		}

		I GetIteratorValue(const Q& Value) const
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndex(Hash);
			return Index < Capacity && !Data[Index].IsFree() ? I(Data, Index, Capacity) : End();
		}

		I GetIteratorValue(const Iterator::IteratorPredicate<Q>& Predicate) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (Predicate(*It))
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
