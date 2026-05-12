#pragma once

#include "NexusFramework/Misc/Sort/Algorithm.h"

namespace NxFr
{
	namespace Sorting
	{
		template<typename T>
		using DefaultIndexed = HeapSort<T>;
		template<typename T, typename N>
		using DefaultLinked = MergeSortLinked<T, N>;
	}

	class Sort
	{
	public:
		template<typename T, typename C, typename S = Sorting::DefaultIndexed<T>>
		static void SortCollection(C& Data, uint64 Count, const Sorting::CompareFunction<T>& Comparison = nullptr)
		{
			if (Count <= 1)
			{
				return;
			}

			S Sorter(Comparison);
			Sorter.Sort(Data, Count);
		}

		template<typename T, typename N, typename S = Sorting::DefaultLinked<T, N>>
		static void SortNodes(N** Data, const Delegate<void(N*, N*)>& SetNext, const Delegate<N*(N*)>& GetNext, const Delegate<const T&(N*)>& GetValue, const Sorting::CompareFunction<T>& Comparison = nullptr)
		{
			if (!Data)
			{
				return;
			}

			S Sorter(SetNext, GetNext, GetValue, Comparison);
			*Data = Sorter.Sort(*Data);
		}
	};
}

