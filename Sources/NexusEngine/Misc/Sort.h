#pragma once

#include "Types/Numbers/Integer.h"
#include "Types/Functions/Delegate.h"
#include "Misc/References.h"

namespace NxEn
{
	namespace Sorting
	{
		template<typename T>
		using CompareFunction = const Delegate<bool(const T&, const T&)>&;

		class SortingAlgorithm
		{
		protected:
			template<typename T>
			static bool Compare(const T& A, const T& B, CompareFunction<T> Comparison = nullptr)
			{
				return Comparison != nullptr ? Comparison(A, B) : A <= B;
			}
		};

		class MergeSort : public SortingAlgorithm
		{
		public:
			template<typename T, typename C>
			static void SortIndexBased(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				Sort(Data, Count, Comparison);
			}

			template<typename T, typename N>
			static N* SortLinkBased(N* Data, CompareFunction<T> Comparison = nullptr)
			{
				return Sort(Data, Comparison);
			}

		private:
			template<typename T, typename C>
			static void Sort(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				C Copy = Data;
				Split(Data, Copy, 0, Count, Comparison);
			}

			template<typename T, typename N>
			static N* Sort(N* Head, CompareFunction<T> Comparison = nullptr)
			{
				if (!Head || !Head->Next)
				{
					return Head;
				}

				N* Next = Split(Head, Comparison);

				Head = Sort(Head, Comparison);
				Next = Sort(Next, Comparison);

				return Merge(Head, Next, Comparison);
			}

			template<typename T, typename C>
			static void Split(C& Copy, C& Data, uint64 Start, uint64 End, CompareFunction<T> Comparison = nullptr)
			{
				if (End - Start <= 1)
				{
					return;
				}

				uint64 Middle = (Start + End) / 2;

				Split(Data, Copy, Start, Middle, Comparison);
				Split(Data, Copy, Middle, End, Comparison);

				Merge(Copy, Data, Start, End, Middle, Comparison);
			}

			template<typename T, typename N>
			static N* Split(N* Head, CompareFunction<T> Comparison = nullptr)
			{
				N* Fast = Head;
				N* Slow = Head;

				while (!Fast && !Fast->Next)
				{
					Fast = Fast->Next->Next;
					if (Fast != nullptr)
					{
						Slow = Slow->Next;
					}
				}

				N* Temp = Slow->Next;
				Slow->Next = nullptr;
				return Temp;
			}

			template<typename T, typename C>
			static void Merge(C& Copy, C& Data, uint64 Start, uint64 End, uint64 Middle, CompareFunction<T> Comparison = nullptr)
			{
				uint64 I = Start, J = Middle;
				for (uint64 K = Start; K < End; ++K)
				{
					if (I < Middle && (J >= End || Compare(Data[I], Data[J], Comparison)))
					{
						Copy[K] = Data[I];
						++I;
					}
					else
					{
						Copy[K] = Data[J];
						++J;
					}
				}
			}

			template<typename T, typename N>
			static N* Merge(N* Head, N* Next, CompareFunction<T> Comparison = nullptr)
			{
				if (!Head)
				{
					return Next;
				}

				if (!Next)
				{
					return Head;
				}

				if (Compare(Head->Value, Next->Value, Comparison))
				{
					Head->Next = Merge(Head->Next, Next, Comparison);
					return Head;
				}
				else
				{
					Next->Next = Merge(Head, Next->Next, Comparison);
					return Next;
				}
			}
		};

		class QuickSort : public SortingAlgorithm
		{
		public:
			template<typename T, typename C>
			static void SortIndexBased(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				Sort(Data, Count, Comparison);
			}

		private:
			template<typename T, typename C>
			static void Sort(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				Split(Data, 0, Count - 1, Comparison);
			}

			template<typename T, typename C>
			static void Split(C& Data, uint64 Start, uint64 End, CompareFunction<T> Comparison = nullptr)
			{
				if (Start >= End || End - Start < 1)
				{
					return;
				}

				uint64 Pivot = FindPivot(Data, Start, End, Comparison);

				if (Pivot > 0)
				{
					Split(Data, Start, Pivot - 1, Comparison);
				}
				if (Pivot < End)
				{
					Split(Data, Pivot + 1, End, Comparison);
				}
			}

			template<typename T, typename C>
			static uint64 FindPivot(C& Data, uint64 Start, uint64 End, CompareFunction<T> Comparison = nullptr)
			{
				uint64 Current = Start;
				uint64 Pivot = End;

				while (Current < Pivot)
				{
					if (Compare(Data[Current], Data[Pivot], Comparison))
					{
						++Current;
					}
					else
					{
						Swap<T>(Data, Current, Pivot);
						--Pivot;
					}
				}

				return Pivot;
			}

			template<typename T, typename C>
			static void Swap(C& Data, uint64 Current, uint64 Pivot)
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
		};

		class HeapSort : public SortingAlgorithm
		{
		public:
			template<typename T, typename C>
			static void SortIndexBased(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				Sort(Data, Count, Comparison);
			}

			template<typename T, typename C>
			static void Heapify(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				Heap(Data, Count, Comparison);
			}

		private:
			template<typename T, typename C>
			static void Sort(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				Heap(Data, Count, Comparison);

				uint64 Index = Count - 1;
				while (true)
				{
					Swap<T>(Data, Index);
					Swap(Data, Index, 0, Comparison);

					if (Index > 0)
					{
						--Index;
					}
					else
					{
						break;
					}
				}
			}

			template<typename T, typename C>
			static void Heap(C& Data, uint64 Count, CompareFunction<T> Comparison = nullptr)
			{
				uint64 Root = Count / 2 - 1;
				while (true)
				{
					Swap(Data, Count, Root, Comparison);

					if (Root > 0)
					{
						--Root;
					}
					else
					{
						break;
					}
				}
			}

			template<typename T, typename C>
			static void Swap(C& Data, uint64 Count, uint64 Root, CompareFunction<T> Comparison = nullptr)
			{
				uint64 Largest = Root;
				uint64 Left = 2 * Root + 1;
				uint64 Right = 2 * Root + 2;

				if (Left < Count && !Compare(Data[Left], Data[Largest], Comparison))
				{
					Largest = Left;
				}

				if (Right < Count && !Compare(Data[Right], Data[Largest], Comparison))
				{
					Largest = Right;
				}

				if (Largest != Root)
				{
					T Temp = Data[Root];
					Data[Root] = Move(Data[Largest]);
					Data[Largest] = Move(Temp);

					Swap(Data, Count, Largest, Comparison);
				}
			}

			template<typename T, typename C>
			static void Swap(C& Data, uint64 Index)
			{
				T Temp = Data[0];
				Data[0] = Move(Data[Index]);
				Data[Index] = Move(Temp);
			}
		};

		using DefaultIndexBased = HeapSort;
		using DefaultLinkBased = MergeSort;
	}

	class Sort
	{
	public:
		template<typename T, typename S = Sorting::DefaultIndexBased, typename C>
		static void SortIndexBased(C& Data, uint64 Count, Sorting::CompareFunction<T> Comparison = nullptr)
		{
			S::SortIndexBased(Data, Count, Comparison);
		}

		template<typename T, typename S = Sorting::DefaultLinkBased, typename N>
		static void SortLinkBased(N** Data, Sorting::CompareFunction<T> Comparison = nullptr)
		{
			*Data = S::SortLinkBased(*Data, Comparison);
		}
	};
}

