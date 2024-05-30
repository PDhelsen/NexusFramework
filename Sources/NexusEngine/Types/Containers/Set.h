#pragma once

#include "Types/Integer.h"
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
		struct Node
		{
			T Value;
			Node* Next;
		};

	public:
		class Iterator
		{
		public:
			Iterator(Node** Data, uint64 Buckets, Node* Current, uint64 Index)
				: Data(Data), Buckets(Buckets), Current(Current), Index(Index)
			{
				if (Current == nullptr && Index != Buckets)
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

			const T* operator->() const
			{
				return &Current->Value;
			}

			const T& operator*() const
			{
				return Current->Value;
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

		void Append(const T& Value)
		{
			uint64 Index = GetIndex(Value);
			if (GetNode(Index, Value) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}
			
			Node* Instance = Allocate();
			Instance->Value = Value;

			AppendNode(Index, Instance);
		}

		void Append(T&& Value)
		{
			uint64 Index = GetIndex(Value);
			if (GetNode(Index, Value) != nullptr)
			{
				return;
			}

			if (GetLoadFactor() > LoadFactorThreshold)
			{
				Resize(Buckets + Buckets / 2);
			}

			Node* Instance = Allocate();
			Instance->Value = Move(Value);

			AppendNode(Index, Instance);
		}

		void AppendRange(const Set<T, H, LF>& Value)
		{
			for (auto& It : Value)
			{
				Append(It);
			}
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
					uint64 Index = GetIndex(Current->Value);
					AppendNode(Index, Current);

					Current = Next;
				}
			}

			Free(TempArray);
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != nullptr;
		}

		T* Find(const T& Other) const
		{
			Node* Instance = GetNode(Other);
			if (!Instance)
			{
				return nullptr;
			}

			return &Instance->Value;
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