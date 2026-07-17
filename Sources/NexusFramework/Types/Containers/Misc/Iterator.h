#pragma once

#include "NexusFramework/Misc/Iterator/Iterator.h"
#include "NexusFramework/Types/Containers/Misc/Node.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	namespace Iterator
	{
		struct Pointer : public Iterator<Byte, Pointer>
		{
		public:
			Pointer(void* Pointer, uint64 Offset)
				: Data(Pointer), Offset(Offset)
			{

			}

			bool Equals(const Pointer& Other) const
			{
				return Data == Other.Data && Offset == Other.Offset;
			}

			void Increment()
			{
				++Offset;
			}

			void Decrement()
			{
				--Offset;
			}

			Byte& Get()
			{
				return *reinterpret_cast<uint8*>(reinterpret_cast<uint64>(Data) + Offset);
			}

			const Byte& Get() const
			{
				return *reinterpret_cast<uint8*>(reinterpret_cast<uint64>(Data) + Offset);
			}

			uint64 Id() const
			{
				return Offset;
			}

		private:
			void* Data;
			uint64 Offset;
		};

		template<typename T>
		struct Block : public Iterator<T, Block<T>>
		{
		public:
			Block(T* Pointer, uint64 Idx)
				: Data(Pointer), Index(Idx)
			{

			}

			bool Equals(const Block<T>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			void Increment()
			{
				++Index;
			}

			void Decrement()
			{
				--Index;
			}

			T& Get()
			{
				return Data[Index];
			}

			const T& Get() const
			{
				return Data[Index];
			}

			uint64 Id() const
			{
				return Index;
			}

		private:
			T* Data;
			uint64 Index;
		};

		template<typename T, uint64 BS>
		struct Bucket : public Iterator<T, Bucket<T, BS>>
		{
		public:
			inline static const uint64 BucketSize = BS;

			Bucket(T** Pointer, uint64 Front, uint64 BucketIdx, uint64 DataIdx)
				: Data(Pointer), Offset(Front), BucketIndex(BucketIdx), DataIndex(DataIdx)
			{

			}

			bool Equals(const Bucket<T, BS>& Other) const
			{
				return Data == Other.Data && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
			}

			void Increment()
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

			void Decrement()
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

			T& Get()
			{
				return Data[BucketIndex][DataIndex];
			}

			const T& Get() const
			{
				return Data[BucketIndex][DataIndex];
			}

			uint64 Id() const
			{
				return BucketIndex * BucketSize + DataIndex - Offset;
			}

		private:
			T** Data;
			uint64 Offset;
			uint64 BucketIndex;
			uint64 DataIndex;
		};

		template<typename T, uint64 BS>
		struct Stack : public Iterator<T, Stack<T, BS>>
		{
		public:
			inline static const uint64 BucketSize = BS;

			Stack(T** Pointer, uint64 Front, uint64 BucketIdx, uint64 DataIdx)
				: Data(Pointer), Offset(Front), BucketIndex(BucketIdx), DataIndex(DataIdx)
			{

			}

			bool Equals(const Stack<T, BS>& Other) const
			{
				return Data == Other.Data && BucketIndex == Other.BucketIndex && DataIndex == Other.DataIndex;
			}

			void Increment()
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

			void Decrement()
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

			T& Get()
			{
				return Data[BucketIndex][DataIndex];
			}

			const T& Get() const
			{
				return Data[BucketIndex][DataIndex];
			}

			uint64 Id() const
			{
				return BucketIndex * BucketSize + DataIndex - Offset;
			}

		private:
			T** Data;
			uint64 Offset;
			uint64 BucketIndex;
			uint64 DataIndex;
		};

		template<typename T, typename N>
		struct Hashmap : public Iterator<T, Hashmap<T, N>>
		{
		public:
			Hashmap(N* Pointer, uint64 Idx, uint64 Cpct)
				: Data(Pointer), Index(Idx), Capacity(Cpct)
			{
				if (Index < Capacity && Data[Index].Free)
				{
					Increment();
				}
			}

			bool Equals(const Hashmap<T, N>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			void Increment()
			{
				do
				{
					++Index;
				} while (Index < Capacity && Data[Index].Free);
			}

			void Decrement()
			{
				NX_ASSERT(false, Default, "Iterator::Hashmap doesn't support moving backward");
			}

			T& Get()
			{
				return Data[Index].Value;
			}

			const T& Get() const
			{
				return Data[Index].Value;
			}

			uint64 Id() const
			{
				return Index;
			}

		private:
			N* Data;
			uint64 Index;
			uint64 Capacity;
		};

		template<typename T, typename N>
		struct PreAllocated : public Iterator<T, PreAllocated<T, N>>
		{
		public:
			PreAllocated(N* Pointer, uint64 Idx, uint64 Cpct)
				: Data(Pointer), Index(Idx), Capacity(Cpct)
			{
				if (Index < Capacity && Data[Index].Next != nullptr)
				{
					Increment();
				}
			}

			bool Equals(const PreAllocated<T, N>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			void Increment()
			{
				do
				{
					++Index;
				} while (Index < Capacity && Data[Index].Next != nullptr);
			}

			void Decrement()
			{
				NX_ASSERT(false, Default, "Iterator::PreAllocated doesn't support moving backward");
			}

			T& Get()
			{
				return Data[Index].Value;
			}

			const T& Get() const
			{
				return Data[Index].Value;
			}

			uint64 Id() const
			{
				return Index;
			}

		private:
			N* Data;
			uint64 Index;
			uint64 Capacity;
		};

		template<typename T, typename N>
		struct NodeSimple : public Iterator<T, NodeSimple<T, N>>
		{
		public:
			NodeSimple(N* Pointer)
				: Current(Pointer)
			{

			}

			bool Equals(const NodeSimple<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			void Increment()
			{
				if (Current)
				{
					Current = Current->Next;
				}
			}

			void Decrement()
			{
				NX_ASSERT(false, Default, "Iterator::NodeSimple doesn't support moving backward");
			}

			T& Get()
			{
				return Current->Value;
			}

			const T& Get() const
			{
				return Current->Value;
			}

			uint64 Id() const
			{
				NX_ASSERT(false, Default, "Iterator::NodeSimple doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		struct NodeDouble : public Iterator<T, NodeDouble<T, N>>
		{
		public:
			NodeDouble(N* Pointer)
				: Current(Pointer)
			{

			}

			bool Equals(const NodeDouble<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			void Increment()
			{
				if (Current)
				{
					Current = Current->Next;
				}
			}

			void Decrement()
			{
				if (Current)
				{
					Current = Current->Prev;
				}
			}

			T& Get()
			{
				return Current->Value;
			}

			const T& Get() const
			{
				return Current->Value;
			}

			uint64 Id() const
			{
				NX_ASSERT(false, Default, "Iterator::NodeDouble doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		struct NodeTree : public Iterator<T, NodeTree<T, N>>
		{
		public:
			NodeTree(N* Pointer)
				: Current(Pointer)
			{
			}

			bool Equals(const NodeTree<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			void Increment()
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

			void Decrement()
			{
				NX_ASSERT(false, Default, "Iterator::NodeTree doesn't support moving backward");
			}

			T& Get()
			{
				return Current->Value;
			}

			const T& Get() const
			{
				return Current->Value;
			}

			uint64 Id() const
			{
				NX_ASSERT(false, Default, "Iterator::NodeTree doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		struct NodeGraph : public Iterator<T, NodeGraph<T, N>>
		{
		public:
			NodeGraph(N* Pointer)
				: Current(Pointer)
			{

			}

			bool Equals(const NodeGraph<T, N>& Other) const
			{
				return Current == Other.Current;
			}

			void Increment()
			{
				if (Current)
				{
					Current = Current->Next;
				}
			}

			void Decrement()
			{
				NX_ASSERT(false, Default, "Iterator::NodeGraph doesn't support moving backward");
			}

			T& Get()
			{
				return Current->Value;
			}

			const T& Get() const
			{
				return Current->Value;
			}

			uint64 Id() const
			{
				NX_ASSERT(false, Default, "Iterator::NodeGraph doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};
	}
}
