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
	template<typename T, class H = Hashing::Default>
	class Set
	{
		friend class ContainersUtils;

	public:
		using N = Node::NodeHashmap<T>;
		using I = Iterator::IteratorHashmap<const T, N>;

		Set(uint64 Size = DefaultSize, Allocator* Allctr = nullptr)
			: Alloc(nullptr), Capacity(0), Count(0), Data(nullptr)
		{
			ValidateAllocator(Allctr);
			Allocate(Size);
		}

		Set(const Set<T, H>& Other)
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(nullptr)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Set - Copy constructor");

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

		Set(Set<T, H>&& Other) noexcept
			: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Set()
		{
			Clear();
			Free();
		}

		Set<T, H>& operator=(const Set<T, H>& Other)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Set - Assignement operator");

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

		Set<T, H>& operator=(Set<T, H>&& Other) noexcept
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

		bool operator==(const Set<T, H>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Set<T, H>& Other) const
		{
			return !(*this == Other);
		}

		const T& Append(const T& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return Data[Index].Value;
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
			Construct(Index, Hash, Value);
			return Data[Index].Value;
		}

		const T& Append(T&& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndexRead(Hash);
			if (Index != Capacity)
			{
				return Data[Index].Value;
			}

			Resize(++Count);
			Index = GetIndexWrite(Hash);
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
				uint64 Index = GetIndexRead(Hash);
				if (Index != Capacity)
				{
					continue;
				}

				Index = GetIndexWrite(Hash);
				Construct(Index, Hash, *It);
			}

			uint64 Hash = GetHash(*Value.Begin());
			uint64 Index = GetIndexRead(Hash);
			return Data[Index].Value;
		}

		void Remove(const T& Value)
		{
			uint64 Hash = GetHash(Value);
			uint64 Index = GetIndexRead(Hash);
			NEXUS_ASSERT(Index != Capacity, "Could not find value");
			Destruct(Index);
			Resize(--Count);
		}

		void Clear()
		{
			DestructRange(0, Capacity);
			Resize(0);
		}

		I GetIterator(const T& Value)
		{
			uint64 Hash = GetHash(Value);
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

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		I Find(const T& Other) const
		{
			uint64 Hash = GetHash(Other);
			uint64 Index = GetIndexRead(Hash);
			return Index != Capacity ? I(Data, Index, Capacity) : End();
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (N*)Memory::Allocate(sizeof(N) * Capacity, NEXUS_MEMORY_ALIGN, Alloc);

			for (uint64 Index = 0; Index < Capacity; ++Index)
			{
				Data[Index].Hash = 0;
			}
		}

		void Reallocate(uint64 Size)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Set - Reallocate");

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

				uint64 Hash = GetHash(Old.Value);
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
		}

		template<typename... Args>
		void Construct(uint64 Index, uint64 Hash, Args&&... args)
		{
			N& Instance = Data[Index];
			NEXUS_ASSERT(Instance.IsFree(), "Construct on an already occupied slot");
			Memory::Construct<T>(&Instance.Value, args...);
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

		uint64 GetHash(const T& Value) const
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

				NEXUS_LOG(Engine, Warning, "Performance", "Set - Collision");
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
