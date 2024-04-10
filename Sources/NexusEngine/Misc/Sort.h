#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"

namespace NxEn
{
	class SortMerge
	{
	public:
		template<typename T>
		static void Sort(T* Data, uint64 Count)
		{
			T* Copy = (T*)Memory::Allocate(sizeof(T) * Count, NEXUS_MEMORY_ALIGN, Memory::GetActiveAllocator());
			Memory::MemCopy(Data, Copy, sizeof(T) * Count);

			Split(Data, Copy, 0, Count);

			Memory::Free(Copy, Memory::GetActiveAllocator());
		}

	private:
		template<typename T>
		static void Split(T* Copy, T* Data, uint64 Start, uint64 End)
		{
			if (End - Start <= 1)
			{
				return;
			}

			uint64 Middle = (Start + End) / 2;

			Split(Data, Copy, Start, Middle);
			Split(Data, Copy, Middle, End);

			Merge(Copy, Data, Start, End, Middle);
		}

		template<typename T>
		static void Merge(T* Copy, T* Data, uint64 Start, uint64 End, uint64 Middle)
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

