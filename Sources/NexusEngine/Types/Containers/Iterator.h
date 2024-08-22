#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"

namespace NxEn
{
	// TODO: Implementation - Nativs - Iterator
	template<typename T>
	class IteratorBlock
	{
	public:
		IteratorBlock(T* Pointer, uint64 Idx)
			: Data(Pointer), Index(Idx)
		{

		}

		IteratorBlock& operator++()
		{
			Index++;
			return *this;
		}

		IteratorBlock operator++(int32)
		{
			IteratorBlock Temp = *this;
			++(*this);
			return Temp;
		}

		IteratorBlock& operator--()
		{
			Index--;
			return *this;
		}

		IteratorBlock operator--(int32)
		{
			IteratorBlock Temp = *this;
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

		bool operator==(const IteratorBlock& Other) const
		{
			return Data == Other.Data && Index == Other.Index;
		}

		bool operator!=(const IteratorBlock& Other) const
		{
			return !(*this == Other);
		}

		uint64 GetIndex() const { return Index; }

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

		IteratorBucket& operator++()
		{
			MoveToNext(true);
			return *this;
		}

		IteratorBucket operator++(int32)
		{
			IteratorBucket Temp = *this;
			++(*this);
			return Temp;
		}

		IteratorBucket& operator--()
		{
			MoveToNext(false);
			return *this;
		}

		IteratorBucket operator--(int32)
		{
			IteratorBucket Temp = *this;
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

		bool operator==(const IteratorBucket& Other) const
		{
			return Data == Other.Data && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
		}

		bool operator!=(const IteratorBucket& Other) const
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
	class IteratorHashmap
	{
	public:
		IteratorHashmap(N** Data, N* Current, uint64 Buckets, uint64 Index)
			: Data(Data), Current(Current), Buckets(Buckets), Index(Index)
		{
			if (Current == nullptr && Index != Buckets)
			{
				MoveToNext();
			}
		}

		IteratorHashmap& operator++()
		{
			MoveToNext();
			return *this;
		}

		IteratorHashmap operator++(int32)
		{
			IteratorHashmap Temp = *this;
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

		bool operator==(const IteratorHashmap& Other) const
		{
			return Current == Other.Current && Index == Other.Index;
		}

		bool operator!=(const IteratorHashmap& Other) const
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

	template<typename T, typename N>
	class IteratorNodeSimple
	{
	public:
		IteratorNodeSimple(N* Pointer)
			: Current(Pointer)
		{

		}

		IteratorNodeSimple& operator++()
		{
			return Next();
		}

		IteratorNodeSimple operator++(int32)
		{
			IteratorNodeSimple Temp = *this;
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

		bool operator==(const IteratorNodeSimple& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeSimple& Other) const
		{
			return !(*this == Other);
		}

		IteratorNodeSimple& Next()
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
	class IteratorNodeDouble
	{
	public:
		IteratorNodeDouble(N* Pointer)
			: Current(Pointer)
		{

		}

		IteratorNodeDouble& operator++()
		{
			return Next();
		}

		IteratorNodeDouble operator++(int32)
		{
			IteratorNodeDouble Temp = *this;
			++(*this);
			return Temp;
		}

		IteratorNodeDouble& operator--()
		{
			return Previous();
		}

		IteratorNodeDouble operator--(int32)
		{
			IteratorNodeDouble Temp = *this;
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

		bool operator==(const IteratorNodeDouble& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeDouble& Other) const
		{
			return !(*this == Other);
		}

		IteratorNodeDouble& Next()
		{
			if (Current)
			{
				Current = Current->Next;
			}
			return *this;
		}

		IteratorNodeDouble& Previous()
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
	class IteratorNodeTree
	{
	public:
		IteratorNodeTree(N* Pointer)
			: Current(Pointer)
		{
		}

		IteratorNodeTree& operator++()
		{
			MoveToNext();
			return *this;
		}

		IteratorNodeTree operator++(int32)
		{
			IteratorNodeTree Temp = *this;
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

		bool operator==(const IteratorNodeTree& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeTree& Other) const
		{
			return !(*this == Other);
		}

		IteratorNodeTree& Parent()
		{
			if (Current)
			{
				Current = Current->Parent;
			}
			return *this;
		}

		IteratorNodeTree& Sibling()
		{
			if (Current)
			{
				Current = Current->Sibling;
			}
			return *this;
		}

		IteratorNodeTree& Child()
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
	class IteratorNodeGraph
	{
	public:
		IteratorNodeGraph(N* Pointer)
			: Current(Pointer)
		{

		}

		IteratorNodeGraph& operator++()
		{
			if (Current)
			{
				Current = Current->Next;
			}
			return *this;
		}

		IteratorNodeGraph operator++(int32)
		{
			IteratorNodeGraph Temp = *this;
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

		bool operator==(const IteratorNodeGraph& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeGraph& Other) const
		{
			return !(*this == Other);
		}

		IteratorNodeGraph& Connections(NodeGraphConnectionType Type, uint64 Index)
		{
			uint64 Idx = 0;
			NodeGraphConnection<T>* Connect = Current->Connection;
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
}
