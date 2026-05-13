#pragma once

#include "NexusFramework/Misc/Iterator/Iterator.h"
#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Containers/Misc/Node.h"

namespace NxFr
{
	namespace Iterator
	{
		struct IteratorPointer : public Iterator<Byte, IteratorPointer>
		{
		public:
			IteratorPointer(void* Pointer, uint64 Offset)
				: Data(Pointer), Offset(Offset)
			{

			}

			bool Equals(const IteratorPointer& Other) const
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
		struct IteratorBlock : public Iterator<T, IteratorBlock<T>>
		{
		public:
			IteratorBlock(T* Pointer, uint64 Idx)
				: Data(Pointer), Index(Idx)
			{

			}

			bool Equals(const IteratorBlock<T>& Other) const
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
		struct IteratorBucket : public Iterator<T, IteratorBucket<T, BS>>
		{
		public:
			inline static const uint64 BucketSize = BS;

			IteratorBucket(T** Pointer, uint64 Front, uint64 BucketIdx, uint64 DataIdx)
				: Data(Pointer), Offset(Front), BucketIndex(BucketIdx), DataIndex(DataIdx)
			{

			}

			bool Equals(const IteratorBucket<T, BS>& Other) const
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
		struct IteratorStack : public Iterator<T, IteratorStack<T, BS>>
		{
		public:
			inline static const uint64 BucketSize = BS;

			IteratorStack(T** Pointer, uint64 Front, uint64 BucketIdx, uint64 DataIdx)
				: Data(Pointer), Offset(Front), BucketIndex(BucketIdx), DataIndex(DataIdx)
			{

			}

			bool Equals(const IteratorStack<T, BS>& Other) const
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
		struct IteratorHashmap : public Iterator<T, IteratorHashmap<T, N>>
		{
		public:
			IteratorHashmap(N* Pointer, uint64 Idx, uint64 Cpct)
				: Data(Pointer), Index(Idx), Capacity(Cpct)
			{
				if (Data[Index].Free && Index < Capacity)
				{
					Increment();
				}
			}

			bool Equals(const IteratorHashmap<T, N>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			void Increment()
			{
				do
				{
					++Index;
				} while (Data[Index].Free && Index < Capacity);
			}

			void Decrement()
			{
				NEXUS_ASSERT(false, Default, "IteratorHashmap doesn't support moving backward");
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
		struct IteratorPreAllocated : public Iterator<T, IteratorPreAllocated<T, N>>
		{
		public:
			IteratorPreAllocated(N* Pointer, uint64 Idx, uint64 Cpct)
				: Data(Pointer), Index(Idx), Capacity(Cpct)
			{
				if (Data[Index].Next != nullptr && Index < Capacity)
				{
					Increment();
				}
			}

			bool Equals(const IteratorPreAllocated<T, N>& Other) const
			{
				return Data == Other.Data && Index == Other.Index;
			}

			void Increment()
			{
				do
				{
					++Index;
				} while (Data[Index].Next != nullptr && Index < Capacity);
			}

			void Decrement()
			{
				NEXUS_ASSERT(false, Default, "IteratorPreAllocated doesn't support moving backward");
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
		struct IteratorNodeSimple : public Iterator<T, IteratorNodeSimple<T, N>>
		{
		public:
			IteratorNodeSimple(N* Pointer)
				: Current(Pointer)
			{

			}

			bool Equals(const IteratorNodeSimple<T, N>& Other) const
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
				NEXUS_ASSERT(false, Default, "IteratorNodeSimple doesn't support moving backward");
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
				NEXUS_ASSERT(false, Default, "IteratorNodeSimple doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		struct IteratorNodeDouble : public Iterator<T, IteratorNodeDouble<T, N>>
		{
		public:
			IteratorNodeDouble(N* Pointer)
				: Current(Pointer)
			{

			}

			bool Equals(const IteratorNodeDouble<T, N>& Other) const
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
				NEXUS_ASSERT(false, Default, "IteratorNodeDouble doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		struct IteratorNodeTree : public Iterator<T, IteratorNodeTree<T, N>>
		{
		public:
			IteratorNodeTree(N* Pointer)
				: Current(Pointer)
			{
			}

			bool Equals(const IteratorNodeTree<T, N>& Other) const
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
				NEXUS_ASSERT(false, Default, "IteratorNodeTree doesn't support moving backward");
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
				NEXUS_ASSERT(false, Default, "IteratorNodeTree doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};

		template<typename T, typename N>
		struct IteratorNodeGraph : public Iterator<T, IteratorNodeGraph<T, N>>
		{
		public:
			IteratorNodeGraph(N* Pointer)
				: Current(Pointer)
			{

			}

			bool Equals(const IteratorNodeGraph<T, N>& Other) const
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
				NEXUS_ASSERT(false, Default, "IteratorNodeGraph doesn't support moving backward");
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
				NEXUS_ASSERT(false, Default, "IteratorNodeTree doesn't support query the id");
				return -1;
			}

		private:
			N* Current;
		};
	}
}
