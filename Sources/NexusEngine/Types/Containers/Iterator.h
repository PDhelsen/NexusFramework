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
			Iterate();
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
			Reverse();
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorBlock& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const IteratorBlock& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorBlock& Other) const
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
			Index++;
		}

		void Reverse()
		{
			Index--;
		}

		IteratorBlock& Next()
		{
			Next();
			return *this;
		}

		IteratorBlock& Previous()
		{
			Previous();
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

		IteratorBucket& operator++()
		{
			Iterate();
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
			Reverse();
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorBucket& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const IteratorBucket& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorBucket& Other) const
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
				BucketIndex++;
				DataIndex = 0;
			}
			else
			{
				DataIndex++;
			}
		}

		void Reverse()
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

		IteratorBucket& Next()
		{
			Iterate();
			return *this;
		}

		IteratorBucket& Previous()
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
		IteratorHashmap(N** Data, N* Current, uint64 Buckets, uint64 Index)
			: Data(Data), Current(Current), Buckets(Buckets), Index(Index)
		{
			if (Current == nullptr && Index != Buckets)
			{
				Iterate();
			}
		}

		IteratorHashmap& operator++()
		{
			Iterate();
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorHashmap& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const IteratorHashmap& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorHashmap& Other) const
		{
			return Current == Other.Current && Index == Other.Index;
		}

		T& Get() const
		{
			return Current->Value;
		}

		T& Id() const
		{
			return Current->Value;
		}

		void Iterate()
		{
			if(Current && Current->Next != nullptr)
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

		IteratorHashmap& Next()
		{
			Iterate();
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
			Iterate();
			return *this;
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
			return Equals(Other);
		}

		bool operator!=(const IteratorNodeSimple& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorNodeSimple& Other) const
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

		IteratorNodeSimple& Next()
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

		IteratorNodeDouble& operator++()
		{
			Iterate();
			return *this;
		}

		IteratorNodeDouble operator++(int32)
		{
			IteratorNodeDouble Temp = *this;
			++(*this);
			return Temp;
		}

		IteratorNodeDouble& operator--()
		{
			Reverse();
			return *this;
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
			return Equals(Other);
		}

		bool operator!=(const IteratorNodeDouble& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorNodeDouble& Other) const
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

		IteratorNodeDouble& Next()
		{
			Iterate();
			return *this;
		}

		IteratorNodeDouble& Previous()
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

		IteratorNodeTree& operator++()
		{
			Iterate();
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorNodeTree& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const IteratorNodeTree& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorNodeTree& Other) const
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

		IteratorNodeTree& Next()
		{
			Next();
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
			Iterate();
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
			return &Get();
		}

		T& operator*() const
		{
			return Get();
		}

		bool operator==(const IteratorNodeGraph& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const IteratorNodeGraph& Other) const
		{
			return !Equals(Other);
		}

		bool Equals(const IteratorNodeGraph& Other) const
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

		IteratorNodeGraph& Next()
		{
			Iterate();
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
