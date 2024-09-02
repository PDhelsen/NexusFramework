#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	class ContainersUtils
	{
	public:
		template<typename T, class H = Fnv1a64>
		static void SetUnion(Set<T, H>& Base, const Set<T, H>& Other)
		{
			Base.Resize(Base.GetCount() + Other.GetCount());

			for (typename Set<T, H>::Iterator It = Other.Begin(); It != Other.End(); It++)
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

		template<typename T, class H = Fnv1a64>
		static void SetDifference(Set<T, H>& Base, const Set<T, H>& Other)
		{
			for (typename Set<T, H>::Iterator It = Other.Begin(); It != Other.End(); It++)
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

		template<typename T, class H = Fnv1a64>
		static Set<T, H> SetIntersection(Set<T, H>& Base, const Set<T, H>& Other)
		{
			Set<T, H> Return = Set<T, H>(NextPrime(Base.GetCount() + Other.GetCount()));

			for (typename Set<T, H>::Iterator It = Base.Begin(); It != Base.End(); It++)
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

			return Return;
		}
	};
}
