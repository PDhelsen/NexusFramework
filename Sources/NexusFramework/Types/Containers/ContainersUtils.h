#pragma once

#include "NexusFramework/Types/Containers/Node.h"
#include "NexusFramework/Types/Containers/Iterator.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dequeue.h"
#include "NexusFramework/Types/Containers/Stack.h"
#include "NexusFramework/Types/Containers/Queue.h"
#include "NexusFramework/Types/Containers/LinkedList.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Tree.h"
#include "NexusFramework/Types/Containers/Graph.h"
#include "NexusFramework/Types/Containers/Collection.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Types/Containers/Tuple.h"

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

		template<typename T>
		static void Resize(Array<T>& Container, uint64 Size)
		{
			uint64 Count = Math::Min(Container.GetCount(), Size);
			Array<T> Result = Array<T>(Size);
			
			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Result[Index] = Container[Index];
			}

			Container = Move(Result);
		}

		template<typename T>
		static void Resize(List<T>& Container, uint64 Size)
		{
			if (Size > Container.GetCount())
			{
				uint64 Delta = Size - Container.GetCount();
				while (Delta-- > 0)
				{
					Container.AppendConstruct();
				}
			}
			else if(Size < Container.GetCount())
			{
				uint64 Delta = Container.GetCount() - Size;
				Container.RemoveRange(Size, Delta);
			}
		}

		template<typename T, typename C>
		static Array<T> ToArray(const C& Container)
		{
			uint64 Index = 0;
			Array<T> Result = Array<T>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It);
			}

			return Result;
		}

		template<typename K, typename T>
		static Array<KeyValuePair<K, T>> ToArray(const Dictionary<K, T>& Container)
		{
			uint64 Index = 0;
			Array<KeyValuePair<K, T>> Result = Array<KeyValuePair<K, T>>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It.Key, It.Value);
			}

			return Result;
		}

		template<typename K, typename T>
		static Array<K> ToArrayKeys(const Dictionary<K, T>& Container)
		{
			uint64 Index = 0;
			Array<K> Result = Array<K>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It.Key);
			}

			return Result;
		}

		template<typename K, typename T>
		static Array<T> ToArrayValues(const Dictionary<K, T>& Container)
		{
			uint64 Index = 0;
			Array<T> Result = Array<T>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It.Value);
			}

			return Result;
		}

		template<typename T, class H = Hashing::Default>
		static void SetUnion(Set<T, H>& Base, const Set<T, H>& Other)
		{
			for (typename Set<T, H>::I It = Other.Begin(); It != Other.End(); ++It)
			{
				uint64 Hash = Base.GetHash(*It);
				uint64 Index = Base.GetIndex(Hash);

				if (Index < Base.Capacity  && !Base.Data[Index].Free)
				{
					continue;
				}
				if (Base.Resize(++Base.Count))
				{
					Index = Base.GetIndex(Hash);
				}

				Base.Construct(Index, Hash, *It);
			}
		}

		template<typename T, class H = Hashing::Default>
		static void SetDifference(Set<T, H>& Base, const Set<T, H>& Other)
		{
			for (typename Set<T, H>::I It = Other.Begin(); It != Other.End(); ++It)
			{
				uint64 Hash = Base.GetHash(*It);
				uint64 Index = Base.GetIndex(Hash);

				if (Base.Data[Index].Free)
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
				uint64 Index = Other.GetIndex(Hash);

				if (!Other.Data[Index].Free)
				{
					continue;
				}

				Hash = Base.GetHash(*It);
				Index = Base.GetIndex(Hash);
				Base.Destruct(Index);
				Base.Resize(--Base.Count);
			}
		}
	};
}
