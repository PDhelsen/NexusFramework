#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"

namespace NxEn
{
	class Sort
	{
	public:
		template<typename T>
		static void MergeSort(T* Data, uint64 Count)
		{
			MergeSortSort(Data, Count);
		}

	private:
		template<typename T>
		static void MergeSortSort(T* Data, uint64 Count)
		{
			T* Copy = (T*)Memory::Allocate(sizeof(T) * Count, NEXUS_MEMORY_ALIGN, Memory::GetActiveAllocator());
			Memory::MemCopy(Data, Copy, sizeof(T) * Count);

			MergeSortSplit(Data, Copy, 0, Count);

			Memory::Free(Copy, Memory::GetActiveAllocator());
		}

		template<typename T>
		static void MergeSortSplit(T* Copy, T* Data, uint64 Start, uint64 End)
		{
			if (End - Start <= 1)
			{
				return;
			}

			uint64 Middle = (Start + End) / 2;

			MergeSortSplit(Data, Copy, Start, Middle);
			MergeSortSplit(Data, Copy, Middle, End);

			MergeSortMerge(Copy, Data, Start, End, Middle);
		}

		template<typename T>
		static void MergeSortMerge(T* Copy, T* Data, uint64 Start, uint64 End, uint64 Middle)
		{
			uint64 I = Start, J = Middle;
			for (uint64 K = Start; K < End; K++)
			{
				if (I < Middle && (J >= End || Data[I] <= Data[J]))
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
	};
}

