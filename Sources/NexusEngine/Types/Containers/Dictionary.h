#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"
#include "Misc/Hash.h"
#include "Misc/Misc.h"

namespace NxEn
{
	// TODO: Implementation - Containers - Union / Intersect / Exclude
	// TODO: Implementation - Containers - Open Addressing
	template<typename K, typename T, class H = Fnv1a64>
	class Dictionary
	{
	public:
		using Node = NodeHashmap<KeyValuePair<K, T>>;
		using Iterator = IteratorHashmap<KeyValuePair<K, T>, Node>;

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

			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Node& Instance = Other.Data[Index];
				if (!Instance.Free)
				{
					Construct(Index, Other.Data[Index].Value);
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

			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Node& Instance = Other.Data[Index];
				if (!Instance.Free)
				{
					Construct(Index, Other.Data[Index].Value);
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

			uint64 Index = GetIndexRead(Key);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			Node& Instance = Data[Index];
			Instance.Value.SetValue(Value);
			return Instance.Value.GetValue();
		}

		T& Assign(const K& Key, T&& Value)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Index = GetIndexRead(Key);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			Node& Instance = Data[Index];
			Instance.Value.SetValue(Move(Value));
			return Instance.Value.GetValue();
		}

		template<typename... Args>
		T& AssignConstruct(const K& Key, Args&&... args)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Index = GetIndexRead(Key);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			Node& Instance = Data[Index];
			Instance.Value.SetValueConstruct(args...);
			return Instance.Value.GetValue();
		}

		template<typename C>
		T& AssignRange(const C& Value)
		{
			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				uint64 Index = GetIndexRead(It->GetKey());
				NEXUS_ASSERT(Index < Capacity, "Failed to find key");
				Data[Index].Value->SetValue(It->GetValue());
			}

			uint64 Index = GetIndexRead(Value.Begin()->GetKey());
			return Data[Index].Value.GetValue();
		}

		T& Append(const K& Key, const T& Value)
		{
			uint64 Index = GetIndexRead(Key);
			if (Index != Capacity)
			{
				return Data[Index].Value.GetValue();
			}

			Resize(++Count);
			Index = GetIndexWrite(Key);
			Construct(Index, Key, Value);
			return Data[Index].Value.GetValue();
		}

		T& Append(K&& Key, T&& Value)
		{
			uint64 Index = GetIndexRead(Key);
			if (Index != Capacity)
			{
				return Data[Index].Value.GetValue();
			}

			Resize(++Count);
			Index = GetIndexWrite(Key);
			Construct(Index, Move(Key), Move(Value));
			return Data[Index].Value.GetValue();
		}

		template<typename... Args>
		T& AppendConstruct(K&& Key, Args&&... args)
		{
			uint64 Index = GetIndexRead(Key);
			if (Index != Capacity)
			{
				return Data[Index].Value.GetValue();
			}

			Resize(++Count);
			Index = GetIndexWrite(Key);
			Construct(Index, Move(Key), args...);
			return Data[Index].Value.GetValue();
		}

		template<typename C>
		T& AppendRange(const C& Value)
		{
			Resize(GetCount() + Value.GetCount());

			for (typename C::Iterator It = Value.Begin(); It != Value.End(); It++)
			{
				uint64 Index = GetIndexRead(It->GetKey());
				if (Index != Capacity)
				{
					continue;
				}

				Index = GetIndexWrite(It->GetKey());
				Construct(Index, It->GetKey(), It->GetValue());
			}

			uint64 Index = GetIndexRead(Value.Begin()->GetKey());
			return Data[Index].Value.GetValue();
		}

		void Remove(const K& Key)
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Index = GetIndexRead(Key);
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

			uint64 Index = GetIndexRead(Key);
			NEXUS_ASSERT(Index < Capacity, "Failed to find key");
			return Data[Index].Value.GetValue();
		}

		T* TryGet(const K& Key) const
		{
			NEXUS_ASSERT(!IsEmpty(), "Dictionary is empty");

			uint64 Index = GetIndexRead(Key);
			if (Index >= Capacity)
			{
				return nullptr;
			}
			return &Data[Index].Value.GetValue();
		}

		Iterator GetIterator(const K& Key)
		{
			uint64 Index = GetIndexRead(Key);
			return Index != Capacity ? Iterator(Data, Index, Capacity) : End();
		}

		Iterator begin() const { return Begin(); }
		Iterator Begin() const
		{
			return Iterator(Data, 0, Capacity);
		}

		Iterator end() const { return End(); }
		Iterator End() const
		{
			return Iterator(Data, Capacity, Capacity);
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

		Iterator FindKey(const K& Key) const
		{
			uint64 Index = GetIndexRead(Key);
			return Index != Capacity ? Iterator(Data, Index, Capacity) : End();
		}

		Iterator FindValue(const T& Value) const
		{
			for (Iterator It = Begin(); It != End(); It++)
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
			Data = (Node*)Memory::Allocate(sizeof(Node) * Capacity, NEXUS_MEMORY_ALIGN, Alloc);

			for (uint64 Index = 0; Index < Capacity; Index++)
			{
				Data[Index].Free = true;
			}
		}

		void Reallocate(uint64 Size)
		{
			NEXUS_LOG(Engine, Warning, "Performance", "Dictionary - Reallocate");

			Node* Temp = Data;
			uint64 Length = Capacity;

			Allocate(Size);

			for (uint64 OldIndex = 0; OldIndex < Length; OldIndex++)
			{
				Node& Old = Temp[OldIndex];
				if (Old.Free)
				{
					continue;
				}

				uint64 NewIndex = GetIndexWrite(Old.Value.GetKey());
				Node& New = Data[NewIndex];

				New.Value = Move(Old.Value);
				New.Free = false;
			}

			Memory::Free(Temp, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			Node& Instance = Data[Index];
			NEXUS_ASSERT(Instance.Free, "Construct on an already occupied slot");
			Memory::Construct<KeyValuePair<K, T>>(&Instance.Value, args...);
			Instance.Free = false;
		}

		void Destruct(uint64 Index)
		{
			Node& Instance = Data[Index];
			NEXUS_ASSERT(!Instance.Free, "Destruct on a free slot");
			Memory::Destruct(&Instance.Value);
			Instance.Free = true;
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; Offset++)
			{
				Node& Instance = Data[Index + Offset];
				if (!Instance.Free)
				{
					Memory::Destruct(&Instance.Value);
					Instance.Free = true;
				}
			}
		}

		uint64 GetIndexHash(const K& Key) const
		{
			uint64 Index = Hash<K, H>::HashObject(Key) % Capacity;
			return Index;
		}

		uint64 GetIndexWrite(const K& Key) const
		{
			uint64 IndexHashed = GetIndexHash(Key);

			uint64 Iteration = 0;
			uint64 Index = IndexHashed;

			while (!Data[Index].Free)
			{
				Index = ProbingPolicy(IndexHashed, ++Iteration);
				NEXUS_ASSERT(Iteration < Capacity, "Failed to find a free spot");

				NEXUS_LOG(Engine, Warning, "Performance", "Dictionary - Collision");
			}

			return Index;
		}

		uint64 GetIndexRead(const K& Key) const
		{
			uint64 IndexHashed = GetIndexHash(Key);

			uint64 Iteration = 0;
			uint64 Index = IndexHashed;

			while (Data[Index].Free || Data[Index].Value.GetKey() != Key)
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
			return NextPrime(Capacity);
		}

		uint64 ProbingPolicy(uint64 Index, uint64 Iteration) const
		{
			bool Flip = Iteration % 2 == 0;
			uint64 Offset = (uint64)ceil((float)(Iteration) / 2.0f);
			Index = Index + (Flip ? -1 : 1) * (Offset * Offset);
			Index = Modulo(Index, Capacity);
			return Index;
		}

		inline static const uint64 DefaultSize = 11;

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		Node* Data;
	};
}
