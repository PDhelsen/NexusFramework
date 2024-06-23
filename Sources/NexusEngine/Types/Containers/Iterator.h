#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"

namespace NxEn
{
	// TODO: Project - Nativs - Iterator
	template<typename T>
	class BlockIterator
	{
	public:
		BlockIterator(T* Pointer, uint64 Idx)
			: Data(Pointer), Index(Idx)
		{

		}

		BlockIterator& operator++()
		{
			Index++;
			return *this;
		}

		BlockIterator operator++(int32)
		{
			BlockIterator Temp = *this;
			++(*this);
			return Temp;
		}

		BlockIterator& operator--()
		{
			Index--;
			return *this;
		}

		BlockIterator operator--(int32)
		{
			BlockIterator Temp = *this;
			--(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Data[Index];
		}

		T& operator*() const
		{
			return Data[Index];
		}

		bool operator==(const BlockIterator& Other) const
		{
			return Data == Other.Data && Index == Other.Index;
		}

		bool operator!=(const BlockIterator& Other) const
		{
			return !(*this == Other);
		}

		uint64 GetIndex() const { return Index; }

	private:
		T* Data;
		uint64 Index;
	};

	template<typename T, uint64 BS>
	class BucketIterator
	{
	public:
		BucketIterator(T** Pointer, uint64 Front, uint64 BucketIdx, uint64 DataIdx)
			: Data(Pointer), Offset(Front), BucketIndex(BucketIdx), DataIndex(DataIdx)
		{

		}

		BucketIterator& operator++()
		{
			MoveToNext(true);
			return *this;
		}

		BucketIterator operator++(int32)
		{
			BucketIterator Temp = *this;
			++(*this);
			return Temp;
		}

		BucketIterator& operator--()
		{
			MoveToNext(false);
			return *this;
		}

		BucketIterator operator--(int32)
		{
			BucketIterator Temp = *this;
			--(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Data[BucketIndex][DataIndex];
		}

		T& operator*() const
		{
			return Data[BucketIndex][DataIndex];
		}

		bool operator==(const BucketIterator& Other) const
		{
			return Data == Other.Data && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
		}

		bool operator!=(const BucketIterator& Other) const
		{
			return !(*this == Other);
		}

		uint64 GetIndex() const { return BucketIndex * BucketSize + DataIndex - Offset; }

	private:
		void MoveToNext(bool Forward)
		{
			if (Forward)
			{
				if (DataIndex == BucketSize - 1)
				{
					BucketIndex++;
					DataIndex = 0;
				}
				else
				{
					DataIndex++;
				}
			}
			else
			{
				if (DataIndex == 0)
				{
					BucketIndex--;
					DataIndex = BucketSize - 1;
				}
				else
				{
					DataIndex--;
				}
			}
		}

		inline static const uint64 BucketSize = BS;

		T** Data;
		uint64 Offset;
		uint64 BucketIndex;
		uint64 DataIndex;
	};

	template<typename T, typename N>
	class LinkedIteratorSimple
	{
	public:
		LinkedIteratorSimple(N* Pointer)
			: Current(Pointer)
		{

		}

		LinkedIteratorSimple& operator++()
		{
			return Next();
		}

		LinkedIteratorSimple operator++(int32)
		{
			LinkedIteratorSimple Temp = *this;
			++(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Current->Value;
		}

		T& operator*() const
		{
			return Current->Value;
		}

		bool operator==(const LinkedIteratorSimple& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const LinkedIteratorSimple& Other) const
		{
			return !(*this == Other);
		}

		LinkedIteratorSimple& Next()
		{
			if (Current)
			{
				Current = Current->Next;
			}
			return *this;
		}

	private:
		N* Current;
	};

	template<typename T, typename N>
	class LinkedIteratorDouble
	{
	public:
		LinkedIteratorDouble(N* Pointer)
			: Current(Pointer)
		{

		}

		LinkedIteratorDouble& operator++()
		{
			return Next();
		}

		LinkedIteratorDouble operator++(int32)
		{
			LinkedIteratorDouble Temp = *this;
			++(*this);
			return Temp;
		}

		LinkedIteratorDouble& operator--()
		{
			return Previous();
		}

		LinkedIteratorDouble operator--(int32)
		{
			LinkedIteratorDouble Temp = *this;
			--(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Current->Value;
		}

		T& operator*() const
		{
			return Current->Value;
		}

		bool operator==(const LinkedIteratorDouble& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const LinkedIteratorDouble& Other) const
		{
			return !(*this == Other);
		}

		LinkedIteratorDouble& Next()
		{
			if (Current)
			{
				Current = Current->Next;
			}
			return *this;
		}

		LinkedIteratorDouble& Previous()
		{
			if (Current)
			{
				Current = Current->Prev;
			}
			return *this;
		}

	private:
		N* Current;
	};

	template<typename T, typename N>
	class LinkedIteratorTree
	{
	public:
		LinkedIteratorTree(N* Pointer)
			: Current(Pointer)
		{
		}

		LinkedIteratorTree& operator++()
		{
			MoveToNext();
			return *this;
		}

		LinkedIteratorTree operator++(int32)
		{
			LinkedIteratorTree Temp = *this;
			++(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Current->Value;
		}

		T& operator*() const
		{
			return Current->Value;
		}

		bool operator==(const LinkedIteratorTree& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const LinkedIteratorTree& Other) const
		{
			return !(*this == Other);
		}

		LinkedIteratorTree& Parent()
		{
			if (Current)
			{
				Current = Current->Parent;
			}
			return *this;
		}

		LinkedIteratorTree& Sibling()
		{
			if (Current)
			{
				Current = Current->Sibling;
			}
			return *this;
		}

		LinkedIteratorTree& Child()
		{
			if (Current)
			{
				Current = Current->Child;
			}
			return *this;
		}

	private:
		void MoveToNext()
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

		N* Current;
	};

	template<typename T, typename N>
	class LinkedIteratorGraph
	{
	public:
		LinkedIteratorGraph(N* Pointer)
			: Current(Pointer)
		{

		}

		LinkedIteratorGraph& operator++()
		{
			if (Current)
			{
				Current = Current->Next;
			}
			return *this;
		}

		LinkedIteratorGraph operator++(int32)
		{
			LinkedIteratorGraph Temp = *this;
			++(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Current->Value;
		}

		T& operator*() const
		{
			return Current->Value;
		}

		bool operator==(const LinkedIteratorGraph& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const LinkedIteratorGraph& Other) const
		{
			return !(*this == Other);
		}

		LinkedIteratorGraph& Connections(LinkedConnectionType Type, uint64 Index)
		{
			uint64 Idx = 0;
			LinkedConnectionGraph<T>* Connect = Current->Connection;
			while (Connect)
			{
				if (Connect->Type == Type)
				{
					if (Idx == Index)
					{
						break;
					}

					Idx++;
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

	template<typename T, typename N>
	class HashmapIterator
	{
	public:
		HashmapIterator(N** Data, N* Current, uint64 Buckets, uint64 Index)
			: Data(Data), Current(Current), Buckets(Buckets), Index(Index)
		{
			if (Current == nullptr && Index != Buckets)
			{
				MoveToNext();
			}
		}

		HashmapIterator& operator++()
		{
			MoveToNext();
			return *this;
		}

		HashmapIterator operator++(int32)
		{
			HashmapIterator Temp = *this;
			++(*this);
			return Temp;
		}

		T* operator->() const
		{
			return &Current->Value;
		}

		T& operator*() const
		{
			return Current->Value;
		}

		bool operator==(const HashmapIterator& Other) const
		{
			return Current == Other.Current && Index == Other.Index;
		}

		bool operator!=(const HashmapIterator& Other) const
		{
			return !(*this == Other);
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

		N** Data;
		N* Current;
		uint64 Buckets;
		uint64 Index;
	};
}
