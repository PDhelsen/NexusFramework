#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"

namespace NxEn
{
	namespace Iterator
	{
		template<typename T>
		class IteratorBlock
		{
		public:
			IteratorBlock(T* Pointer, uint64 Idx)
				: Data(Pointer), Index(Idx)
			{

			}

			IteratorBlock<T>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorBlock<T> operator++(int32)
			{
				IteratorBlock<T> Temp = *this;
				++(*this);
				return Temp;
			}

			IteratorBlock<T>& operator--()
			{
				Reverse();
				return *this;
			}

			IteratorBlock<T> operator--(int32)
			{
				IteratorBlock<T> Temp = *this;
				--(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorBlock<T>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorBlock<T>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorBlock<T>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			T& Get() const
			{
				return Data[Index];
			}

			uint64 Id() const
			{
				return Index;
			}

			void Iterate()
			{
				++Index;
			}

			void Reverse()
			{
				--Index;
			}

			IteratorBlock<T>& Next()
			{
				Iterate();
				return *this;
			}

			IteratorBlock<T>& Previous()
			{
				Reverse();
				return *this;
			}

		private:
			T* Data;
			uint64 Index;
		};

		template<typename T, uint64 BS>
		class IteratorBucket
		{
		public:
			IteratorBucket(T** Pointer, uint64 Front, uint64 BucketIdx, uint64 DataIdx)
				: Data(Pointer), Offset(Front), BucketIndex(BucketIdx), DataIndex(DataIdx)
			{

			}

			IteratorBucket<T, BS>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorBucket<T, BS> operator++(int32)
			{
				IteratorBucket<T, BS> Temp = *this;
				++(*this);
				return Temp;
			}

			IteratorBucket<T, BS>& operator--()
			{
				Reverse();
				return *this;
			}

			IteratorBucket<T, BS> operator--(int32)
			{
				IteratorBucket<T, BS> Temp = *this;
				--(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorBucket<T, BS>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorBucket<T, BS>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorBucket<T, BS>& Other) const
			{
				return Data == Other.Data && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
			}

			T& Get() const
			{
				return Data[BucketIndex][DataIndex];
			}

			uint64 Id() const
			{
				return BucketIndex * BucketSize + DataIndex - Offset;
			}

			void Iterate()
			{
				if (DataIndex == BucketSize - 1)
				{
					++BucketIndex;
					DataIndex = 0;
				}
				else
				{
					++DataIndex;
				}
			}

			void Reverse()
			{
				if (DataIndex == 0)
				{
					--BucketIndex;
					DataIndex = BucketSize - 1;
				}
				else
				{
					--DataIndex;
				}
			}

			IteratorBucket<T, BS>& Next()
			{
				Iterate();
				return *this;
			}

			IteratorBucket<T, BS>& Previous()
			{
				Reverse();
				return *this;
			}

		private:
			inline static const uint64 BucketSize = BS;

			T** Data;
			uint64 Offset;
			uint64 BucketIndex;
			uint64 DataIndex;
		};

		template<typename T, typename N>
		class IteratorHashmap
		{
		public:
			IteratorHashmap(N* Pointer, uint64 Idx, uint64 Cpct)
				: Data(Pointer), Index(Idx), Capacity(Cpct)
			{
				if (Data[Index].IsFree() && Index < Capacity)
				{
					Iterate();
				}
			}

			IteratorHashmap<T, N>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorHashmap<T, N> operator++(int32)
			{
				IteratorHashmap<T, N> Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorHashmap<T, N>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorHashmap<T, N>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorHashmap<T, N>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			T& Get() const
			{
				return Data[Index].Value;
			}

			uint64 Id() const
			{
				return Index;
			}

			void Iterate()
			{
				do
				{
					++Index;
				} while (Data[Index].IsFree() && Index < Capacity);
			}

			IteratorHashmap<T, N>& Next()
			{
				Iterate();
				return *this;
			}

		private:
			N* Data;
			uint64 Index;
			uint64 Capacity;
		};

		template<typename T, typename N>
		class IteratorPreAllocated
		{
		public:
			IteratorPreAllocated(N* Pointer, uint64 Idx, uint64 Cpct)
				: Data(Pointer), Index(Idx), Capacity(Cpct)
			{
				if (Data[Index].Next != nullptr && Index < Capacity)
				{
					Iterate();
				}
			}

			IteratorPreAllocated<T, N>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorPreAllocated<T, N> operator++(int32)
			{
				IteratorPreAllocated<T, N> Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorPreAllocated<T, N>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorPreAllocated<T, N>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorPreAllocated<T, N>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			T& Get() const
			{
				return Data[Index].Value;
			}

			uint64 Id() const
			{
				return Index;
			}

			void Iterate()
			{
				do
				{
					++Index;
				} while (Data[Index].Next != nullptr && Index < Capacity);
			}

			IteratorPreAllocated<T, N>& Next()
			{
				Iterate();
				return *this;
			}


		private:
			N* Data;
			uint64 Index;
			uint64 Capacity;
		};

		template<typename T, typename N>
		class IteratorNodeSimple
		{
		public:
			IteratorNodeSimple(N* Pointer)
				: Current(Pointer)
			{

			}

			IteratorNodeSimple<T, N>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorNodeSimple<T, N> operator++(int32)
			{
				IteratorNodeSimple<T, N> Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorNodeSimple<T, N>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorNodeSimple<T, N>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorNodeSimple<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			T& Get() const
			{
				return Current->Value;
			}

			T* Id() const
			{
				return &Current->Value;
			}

			void Iterate()
			{
				if (Current)
				{
					Current = Current->Next;
				}
			}

			IteratorNodeSimple<T, N>& Next()
			{
				Iterate();
				return *this;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		class IteratorNodeDouble
		{
		public:
			IteratorNodeDouble(N* Pointer)
				: Current(Pointer)
			{

			}

			IteratorNodeDouble<T, N>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorNodeDouble<T, N> operator++(int32)
			{
				IteratorNodeDouble<T, N> Temp = *this;
				++(*this);
				return Temp;
			}

			IteratorNodeDouble<T, N>& operator--()
			{
				Reverse();
				return *this;
			}

			IteratorNodeDouble<T, N> operator--(int32)
			{
				IteratorNodeDouble<T, N> Temp = *this;
				--(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorNodeDouble<T, N>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorNodeDouble<T, N>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorNodeDouble<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			T& Get() const
			{
				return Current->Value;
			}

			T* Id() const
			{
				return &Current->Value;
			}

			void Iterate()
			{
				if (Current)
				{
					Current = Current->Next;
				}
			}

			void Reverse()
			{
				if (Current)
				{
					Current = Current->Prev;
				}
			}

			IteratorNodeDouble<T, N>& Next()
			{
				Iterate();
				return *this;
			}

			IteratorNodeDouble<T, N>& Previous()
			{
				Reverse();
				return *this;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		class IteratorNodeTree
		{
		public:
			IteratorNodeTree(N* Pointer)
				: Current(Pointer)
			{
			}

			IteratorNodeTree<T, N>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorNodeTree<T, N> operator++(int32)
			{
				IteratorNodeTree<T, N> Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorNodeTree<T, N>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorNodeTree<T, N>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorNodeTree<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			T& Get() const
			{
				return Current->Value;
			}

			T* Id() const
			{
				return &Current->Value;
			}

			void Iterate()
			{
				if (Current && Current->Child)
				{
					Current = Current->Child;
				}
				else if (Current && Current->Sibling)
				{
					Current = Current->Sibling;
				}
				else
				{
					while (Current)
					{
						if (Current->Parent && Current->Parent->Sibling)
						{
							Current = Current->Parent->Sibling;
							break;
						}

						Current = Current->Parent;
					}
				}
			}

			IteratorNodeTree<T, N>& Next()
			{
				Iterate();
				return *this;
			}

			IteratorNodeTree<T, N>& Parent()
			{
				if (Current)
				{
					Current = Current->Parent;
				}
				return *this;
			}

			IteratorNodeTree<T, N>& Sibling()
			{
				if (Current)
				{
					Current = Current->Sibling;
				}
				return *this;
			}

			IteratorNodeTree<T, N>& Child()
			{
				if (Current)
				{
					Current = Current->Child;
				}
				return *this;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		class IteratorNodeGraph
		{
		public:
			IteratorNodeGraph(N* Pointer)
				: Current(Pointer)
			{

			}

			IteratorNodeGraph<T, N>& operator++()
			{
				Iterate();
				return *this;
			}

			IteratorNodeGraph<T, N> operator++(int32)
			{
				IteratorNodeGraph<T, N> Temp = *this;
				++(*this);
				return Temp;
			}

			T* operator->() const
			{
				return &Get();
			}

			T& operator*() const
			{
				return Get();
			}

			bool operator==(const IteratorNodeGraph<T, N>& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorNodeGraph<T, N>& Other) const
			{
				return !Equals(Other);
			}

			bool Equals(const IteratorNodeGraph<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			T& Get() const
			{
				return Current->Value;
			}

			T* Id() const
			{
				return &Current->Value;
			}

			void Iterate()
			{
				if (Current)
				{
					Current = Current->Next;
				}
			}

			IteratorNodeGraph<T, N>& Next()
			{
				Iterate();
				return *this;
			}

			IteratorNodeGraph<T, N>& Connections(Node::NodeGraphConnectionType Type, uint64 Index)
			{
				uint64 Idx = 0;
				Node::NodeGraphConnection<T>* Connect = Current->Connection;
				while (Connect)
				{
					if (Connect->Type == Type)
					{
						if (Idx == Index)
						{
							break;
						}

						++Idx;
					}

					Connect = Connect->Next;
				}

				if (Connect)
				{
					Current = Connect->Target;
				}
				else
				{
					Current = nullptr;
				}

				return *this;
			}

		private:
			N* Current;
		};
	}
}
