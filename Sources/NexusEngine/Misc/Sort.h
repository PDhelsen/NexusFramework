#pragma once

#include "Types/Integer.h"
#include "Misc/References.h"

namespace NxEn
{
	// TODO: Architecture - Sort - Strategy Pattern
	class Sort
	{
	public:
		template<typename T>
		using CompareFunction = bool(*)(const T&, const T&);

		template<typename T, typename C>
		static void MergeSort(C& Data, CompareFunction<T> Compare = nullptr)
		{
			Sort::MergeSortSort(Data, Data.GetCount(), Compare);
		}

		template<typename T, typename C>
		static void QuickSort(C& Data, CompareFunction<T> Compare = nullptr)
		{
			Sort::QuickSortSort(Data, Data.GetCount(), Compare);
		}

		template<typename T, typename C>
		static void HeapSort(C& Data, CompareFunction<T> Compare = nullptr)
		{
			Sort::HeapSortSort(Data, Data.GetCount(), Compare);
		}

		template<typename T, typename C>
		static void Heapify(C& Data, CompareFunction<T> Compare = nullptr)
		{
			Sort::HeapifySort(Data, Data.GetCount(), Compare);
		}

		template<typename T, typename N>
		static N* LinkSort(N* Root, CompareFunction<T> Compare = nullptr)
		{
			Sort::LinkSortSort<T, N>(&Root, Compare);
			return Root;
		}

	private:
		template<typename T, typename C>
		static void MergeSortSort(C& Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			C Copy = Data.Copy();
			Sort::MergeSortSplit(Data, Copy, 0, Count, Compare);
		}

		template<typename T, typename C>
		static void MergeSortSplit(C& Copy, C& Data, uint64 Start, uint64 End, CompareFunction<T> Compare = nullptr)
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

		template<typename T, typename C>
		static void MergeSortMerge(C& Copy, C& Data, uint64 Start, uint64 End, uint64 Middle, CompareFunction<T> Compare = nullptr)
		{
			uint64 I = Start, J = Middle;
			for (uint64 K = Start; K < End; K++)
			{
				if (I < Middle && (J >= End || DoCompare(Data[I], Data[J], Compare)))
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
	
		template<typename T, typename C>
		static void QuickSortSort(C& Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			Sort::QuickSortSplit(Data, 0, Count - 1, Compare);
		}

		template<typename T, typename C>
		static void QuickSortSplit(C& Data, uint64 Start, uint64 End, CompareFunction<T> Compare = nullptr)
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

		template<typename T, typename C>
		static uint64 QuickSortPivot(C& Data, uint64 Start, uint64 End, CompareFunction<T> Compare = nullptr)
		{
			uint64 Current = Start;
			uint64 Pivot = End;

			while (Current < Pivot)
			{
				if (DoCompare(Data[Current], Data[Pivot], Compare))
				{
					Current++;
				}
				else
				{
					Sort::QuickSortSwap<T>(Data, Current, Pivot);
					Pivot--;
				}
			}

			return Pivot;
		}

		template<typename T, typename C>
		static void QuickSortSwap(C& Data, uint64 Current, uint64 Pivot)
		{
			T Temp = Data[Current];

			if (Pivot - Current > 1)
			{
				Data[Current] = Move(Data[Pivot - 1]);
				Data[Pivot - 1] = Move(Data[Pivot]);
				Data[Pivot] = Move(Temp);
			}
			else
			{
				Data[Current] = Move(Data[Pivot]);
				Data[Pivot] = Move(Temp);
			}
		}

		template<typename T, typename C>
		static void HeapSortSort(C& Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			Sort::HeapifySort(Data, Count, Compare);

			uint64 Index = Count - 1;
			while (true)
			{
				HeapSortSwap<T>(Data, Index);
				HeapifySwap(Data, Index, 0, Compare);

				if (Index > 0)
				{
					Index--;
				}
				else
				{
					break;
				}
			}
		}

		template<typename T, typename C>
		static void HeapSortSwap(C& Data, uint64 Index)
		{
			T Temp = Data[0];
			Data[0] = Move(Data[Index]);
			Data[Index] = Move(Temp);
		}

		template<typename T, typename C>
		static void HeapifySort(C& Data, uint64 Count, CompareFunction<T> Compare = nullptr)
		{
			uint64 Root = Count / 2 - 1;
			while(true)
			{
				Sort::HeapifySwap(Data, Count, Root, Compare);
			
				if (Root > 0)
				{
					Root--;
				}
				else
				{
					break;
				}
			}
		}

		template<typename T, typename C>
		static void HeapifySwap(C& Data, uint64 Count, uint64 Root, CompareFunction<T> Compare = nullptr)
		{
			uint64 Largest = Root;
			uint64 Left = 2 * Root + 1;
			uint64 Right = 2 * Root + 2;

			if (Left < Count && !DoCompare(Data[Left], Data[Largest], Compare))
			{
				Largest = Left;
			}

			if (Right < Count && !DoCompare(Data[Right], Data[Largest], Compare))
			{
				Largest = Right;
			}

			if (Largest != Root)
			{
				T Temp = Data[Root];
				Data[Root] = Move(Data[Largest]);
				Data[Largest] = Move(Temp);
			
				Sort::HeapifySwap(Data, Count, Largest, Compare);
			}
		}

		template<typename T, typename N>
		static void LinkSortSort(N** HeadRef, CompareFunction<T> Compare = nullptr)
		{
			N* Head = *HeadRef;
			if (Head == nullptr || Head->Next == nullptr)
			{
				return;
			}

			N* A = nullptr;
			N* B = nullptr;
			LinkSortSplit<N>(Head, &A, &B);

			LinkSortSort<T, N>(&A);
			LinkSortSort<T, N>(&B);

			*HeadRef = LinkSortMerge<T, N>(A, B);
		}

		template<typename T, typename N>
		static N* LinkSortMerge(N* A, N* B, CompareFunction<T> Compare = nullptr)
		{
			N* Result = nullptr;

			if (A == nullptr)
			{
				return B;
			}
			else if (B == nullptr)
			{
				return A;
			}

			if (DoCompare(A->Value, B->Value, Compare))
			{
				Result = A;
				Result->Next = LinkSortMerge<T, N>(A->Next, B);
			}
			else
			{
				Result = B;
				Result->Next = LinkSortMerge<T, N>(A, B->Next);
			}

			return Result;
		}

		template<typename N>
		static void LinkSortSplit(N* Source, N** FrontRef, N** BackRef)
		{
			N* Slow;
			N* Fast;

			Slow = Source;
			Fast = Source->Next;

			while (Fast != nullptr)
			{
				Fast = Fast->Next;
				if (Fast != nullptr)
				{
					Slow = Slow->Next;
					Fast = Fast->Next;
				}
			}

			*FrontRef = Source;
			*BackRef = Slow->Next;

			Slow->Next = nullptr;
		}

		template<typename T>
		static bool DoCompare(const T& A, const T& B, CompareFunction<T> Compare = nullptr)
		{
			return Compare != nullptr ? Compare(A, B) : A <= B;
		}
	};
}

