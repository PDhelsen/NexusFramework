#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"

namespace NxEn
{
	class Sort
	{
	public:
		template<typename T>
		using CompareFunction = bool(*)(const T&, const T&);

		template<typename T>
		static void MergeSort(T* Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			Sort::MergeSortSort(Data, Count, Compare);
		}

		template<typename T>
		static void QuickSort(T* Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			Sort::QuickSortSort(Data, Count, Compare);
		}

	private:
		template<typename T>
		static void MergeSortSort(T* Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			T* Copy = (T*)Memory::Allocate(sizeof(T) * Count, NEXUS_MEMORY_ALIGN, Memory::GetActiveAllocator());
			Memory::MemCopy(Data, Copy, sizeof(T) * Count);

			Sort::MergeSortSplit(Data, Copy, 0, Count, Compare);

			Memory::Free(Copy, Memory::GetActiveAllocator());
		}

		template<typename T>
		static void MergeSortSplit(T* Copy, T* Data, uint64 Start, uint64 End, CompareFunction<T> Compare = nullptr)
		{
			if (End - Start <= 1)
			{
				return;
			}

			uint64 Middle = (Start + End) / 2;

			Sort::MergeSortSplit(Data, Copy, Start, Middle, Compare);
			Sort::MergeSortSplit(Data, Copy, Middle, End, Compare);

			Sort::MergeSortMerge(Copy, Data, Start, End, Middle, Compare);
		}

		template<typename T>
		static void MergeSortMerge(T* Copy, T* Data, uint64 Start, uint64 End, uint64 Middle, CompareFunction<T> Compare = nullptr)
		{
			uint64 I = Start, J = Middle;
			for (uint64 K = Start; K < End; K++)
			{
				bool LessThan = Compare != nullptr ? Compare(Data[I], Data[J]) : Data[I] <= Data[J];
				if (I < Middle && (J >= End || LessThan))
				{
					Copy[K] = Data[I];
					I++;
				}
				else
				{
					Copy[K] = Data[J];
					J++;
				}
			}
		}
	
		template<typename T>
		static void QuickSortSort(T* Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			Sort::QuickSortSplit(Data, 0, Count - 1, Compare);
		}

		template<typename T>
		static void QuickSortSplit(T* Data, uint64 Start, uint64 End, CompareFunction<T> Compare = nullptr)
		{
			if (Start >= End || End - Start < 1)
			{
				return;
			}

			uint64 Pivot = Sort::QuickSortPivot(Data, Start, End, Compare);

			if (Pivot > 0)
			{
				Sort::QuickSortSplit(Data, Start, Pivot - 1, Compare);
			}
			if (Pivot < End)
			{
				Sort::QuickSortSplit(Data, Pivot + 1, End, Compare);
			}
		}

		template<typename T>
		static uint64 QuickSortPivot(T* Data, uint64 Start, uint64 End, CompareFunction<T> Compare = nullptr)
		{
			uint64 Current = Start;
			uint64 Pivot = End;

			while (Current < Pivot)
			{
				bool LessThan = Compare != nullptr ? Compare(Data[Current], Data[Pivot]) : Data[Current] <= Data[Pivot];
				if (LessThan)
				{
					Current++;
				}
				else
				{
					Sort::QuickSortSwap(Data, Current, Pivot);
					Pivot--;
				}
			}

			return Pivot;
		}

		template<typename T>
		static void QuickSortSwap(T* Data, uint64 Current, uint64 Pivot)
		{
			T Temp = Data[Current];

			if (Pivot - Current > 1)
			{
				Data[Current] = Data[Pivot - 1];
				Data[Pivot - 1] = Data[Pivot];
				Data[Pivot] = Temp;
			}
			else
			{
				Data[Current] = Data[Pivot];
				Data[Pivot] = Temp;
			}
		}
	};
}

