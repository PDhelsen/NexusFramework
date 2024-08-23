#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"

namespace NxEn
{
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
			return Next();
		}

		IteratorBlock operator++(int32)
		{
			IteratorBlock Temp = *this;
			++(*this);
			return Temp;
		}

		IteratorBlock& operator--()
		{
			return Previous();
		}

		IteratorBlock operator--(int32)
		{
			IteratorBlock Temp = *this;
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

		bool operator==(const IteratorBlock& Other) const
		{
			return Data == Other.Data && Index == Other.Index;
		}

		bool operator!=(const IteratorBlock& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Data[Index];
		}

		void Move()
		{
			Next();
		}

		IteratorBlock& Next()
		{
			Index++;
			return *this;
		}

		IteratorBlock& Previous()
		{
			Index--;
			return *this;
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
			return Next();
		}

		IteratorBucket operator++(int32)
		{
			IteratorBucket Temp = *this;
			++(*this);
			return Temp;
		}

		IteratorBucket& operator--()
		{
			return Previous();
		}

		IteratorBucket operator--(int32)
		{
			IteratorBucket Temp = *this;
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

		bool operator==(const IteratorBucket& Other) const
		{
			return Data == Other.Data && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
		}

		bool operator!=(const IteratorBucket& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Data[BucketIndex][DataIndex];
		}

		void Move()
		{
			Next();
		}

		IteratorBucket& Next()
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

			return *this;
		}

		IteratorBucket& Previous()
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

			return *this;
		}

		uint64 GetIndex() const { return BucketIndex * BucketSize + DataIndex - Offset; }

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
		IteratorHashmap(N** Data, N* Current, uint64 Buckets, uint64 Index)
			: Data(Data), Current(Current), Buckets(Buckets), Index(Index)
		{
			if (Current == nullptr && Index != Buckets)
			{
				Next();
			}
		}

		IteratorHashmap& operator++()
		{
			return Next();
		}

		IteratorHashmap operator++(int32)
		{
			IteratorHashmap Temp = *this;
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

		bool operator==(const IteratorHashmap& Other) const
		{
			return Current == Other.Current && Index == Other.Index;
		}

		bool operator!=(const IteratorHashmap& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Current->Value;;
		}

		void Move()
		{
			Next();
		}

		IteratorHashmap& Next()
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

			return *this;
		}

	private:
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorNodeSimple& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeSimple& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Current->Value;
		}

		void Move()
		{
			Next();
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorNodeDouble& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeDouble& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Current->Value;;
		}

		void Move()
		{
			Next();
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
			return Next();
		}

		IteratorNodeTree operator++(int32)
		{
			IteratorNodeTree Temp = *this;
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

		bool operator==(const IteratorNodeTree& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeTree& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Current->Value;;
		}

		void Move()
		{
			Next();
		}

		IteratorNodeTree& Next()
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

			return *this;
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
			return Next();
		}

		IteratorNodeGraph operator++(int32)
		{
			IteratorNodeGraph Temp = *this;
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

		bool operator==(const IteratorNodeGraph& Other) const
		{
			return Current == Other.Current;
		}

		bool operator!=(const IteratorNodeGraph& Other) const
		{
			return !(*this == Other);
		}

		T& Get() const
		{
			return Current->Value;;
		}

		void Move()
		{
			Next();
		}

		IteratorNodeGraph& Next()
		{
			if (Current)
			{
				Current = Current->Next;
			}

			return *this;
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
