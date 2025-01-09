#pragma once

#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Dequeue.h"
#include "Types/Containers/Stack.h"
#include "Types/Containers/Queue.h"
#include "Types/Containers/LinkedList.h"
#include "Types/Containers/Set.h"
#include "Types/Containers/Dictionary.h"
#include "Types/Containers/Tree.h"
#include "Types/Containers/Graph.h"
#include "Types/Containers/Collection.h"
#include "Types/Containers/Buffer.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/Tuple.h"

namespace NxFr
{
	class ContainersUtils
	{
	public:
		template<typename T, typename C>
		static void Fill(C& Container, const T& Value)
		{
			for (auto& It : Container)
			{
				It = Value;
			}
		}

		template<typename K, typename T>
		static void Fill(Dictionary<K, T>& Container, const T& Value)
		{
			for (auto& It : Container)
			{
				It.Value = Value;
			}
		}

		template<typename T, class H = Hashing::Default>
		static void SetUnion(Set<T, H>& Base, const Set<T, H>& Other)
		{
			Base.Resize(Base.GetCount() + Other.GetCount());

			for (typename Set<T, H>::I It = Other.Begin(); It != Other.End(); ++It)
			{
				uint64 Hash = Base.GetHash(*It);
				uint64 Index = Base.GetIndexRead(Hash);

				if (Index != Base.GetCapacity())
				{
					continue;
				}

				Index = Base.GetIndexWrite(Hash);
				Base.Construct(Index, Hash, *It);
			}
		}

		template<typename T, class H = Hashing::Default>
		static void SetDifference(Set<T, H>& Base, const Set<T, H>& Other)
		{
			for (typename Set<T, H>::I It = Other.Begin(); It != Other.End(); ++It)
			{
				uint64 Hash = Base.GetHash(*It);
				uint64 Index = Base.GetIndexRead(Hash);

				if (Index == Base.GetCapacity())
				{
					continue;
				}

				Base.Destruct(Index);
				Base.Resize(--Base.Count);
			}
		}

		template<typename T, class H = Hashing::Default>
		static void SetIntersection(Set<T, H>& Base, const Set<T, H>& Other)
		{
			for (typename Set<T, H>::I It = Base.Begin(); It != Base.End(); ++It)
			{
				uint64 Hash = Other.GetHash(*It);
				uint64 Index = Other.GetIndexRead(Hash);

				if (Index != Other.GetCapacity())
				{
					continue;
				}

				Hash = Base.GetHash(*It);
				Index = Base.GetIndexRead(Hash);
				Base.Destruct(Index);
				Base.Resize(--Base.Count);
			}
		}
	};
}
