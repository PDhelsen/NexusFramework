#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	namespace Sorting
	{
		template<typename T>
		using CompareFunction = Delegate<bool(const T&, const T&)>;

		template<typename T>
		static bool CompareDefault(const T& A, const T& B)
		{
			return A <= B;
		}

		template<typename T>
		static bool Compare(const T& A, const T& B, const CompareFunction<T>& Comparison = nullptr)
		{
			return !Comparison.IsNull() ? Comparison(A, B) : CompareDefault(A, B);
		}

		template<typename T>
		class MergeSortIndexed
		{
		public:
			MergeSortIndexed(const CompareFunction<T>& Comparison = nullptr)
				: Comparison(Comparison)
			{
			}

			template<typename C>
			void Sort(C& Data, uint64 Count) const
			{
				C Copy = Data;
				Split(Data, Copy, 0, Count);
			}

			template<typename C>
			void Split(C& Copy, C& Data, uint64 Start, uint64 End) const
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

			template<typename C>
			void Merge(C& Copy, C& Data, uint64 Start, uint64 End, uint64 Middle) const
			{
				uint64 C = Start, J = Middle;
				for (uint64 K = Start; K < End; ++K)
				{
					if (C < Middle && (J >= End || Compare(Data[C], Data[J], Comparison)))
					{
						Copy[K] = Data[C];
						++C;
					}
					else
					{
						Copy[K] = Data[J];
						++J;
					}
				}
			}

		private:
			CompareFunction<T> Comparison;
		};

		template<typename T, typename N>
		class MergeSortLinked
		{
		public:
			MergeSortLinked(const Delegate<void(N*, N*)>& SetNext, const Delegate<N* (N*)>& GetNext, const Delegate<const T& (N*)>& GetValue, const CompareFunction<T>& Comparison = nullptr)
				: SetNext(SetNext), GetNext(GetNext), GetValue(), Comparison(Comparison)
			{
			}

			template<typename N>
			N* Sort(N* Head) const
			{
				if (!Head || !GetNext(Head))
				{
					return Head;
				}

				N* Next = Split(Head);

				Head = Sort(Head);
				Next = Sort(Next);

				return Merge(Head, Next);
			}

			template<typename N>
			N* Split(N* Head) const
			{
				N* Fast = Head;
				N* Slow = Head;

				while (!Fast && !GetNext(Fast))
				{
					Fast = GetNext(GetNext(Fast));
					if (Fast != nullptr)
					{
						Slow = GetNext(Slow);
					}
				}

				N* Temp = GetNext(Slow);
				SetNext(Slow, nullptr);
				return Temp;
			}

			template<typename N>
			N* Merge(N* Head, N* Next) const
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
					SetNext(Head, Merge(GetNext(Head), Next));
					return Head;
				}
				else
				{
					SetNext(Next, Merge(Head, GetNext(Next)));
					return Next;
				}
			}

		private:
			Delegate<void(N*, N*)> SetNext;
			Delegate<N* (N*)> GetNext;
			Delegate<const T& (N*)> GetValue;
			CompareFunction<T> Comparison;
		};

		template<typename T>
		class QuickSort
		{
		public:
			QuickSort(const CompareFunction<T>& Comparison = nullptr)
				: Comparison(Comparison)
			{
			}

			template<typename C>
			void Sort(C& Data, uint64 Count) const
			{
				Split(Data, 0, Count - 1);
			}

			template<typename C>
			void Split(C& Data, uint64 Start, uint64 End) const
			{
				if (Start >= End || End - Start < 1)
				{
					return;
				}

				uint64 Pivot = FindPivot(Data, Start, End);

				if (Pivot > 0)
				{
					Split(Data, Start, Pivot - 1);
				}
				if (Pivot < End)
				{
					Split(Data, Pivot + 1, End);
				}
			}

			template<typename C>
			uint64 FindPivot(C& Data, uint64 Start, uint64 End) const
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
						Swap(Data, Current, Pivot);
						--Pivot;
					}
				}

				return Pivot;
			}

			template<typename C>
			void Swap(C& Data, uint64 Current, uint64 Pivot) const
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

		private:
			CompareFunction<T> Comparison;
		};

		template<typename T>
		class HeapSort
		{
		public:
			HeapSort(const CompareFunction<T>& Comparison = nullptr)
				: Comparison(Comparison)
			{
			}

			template<typename C>
			void Sort(C& Data, uint64 Count) const
			{
				Heap(Data, Count);

				uint64 Index = Count - 1;
				while (true)
				{
					Swap(Data, Index);
					Swap(Data, Index, 0);

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

			template<typename C>
			void Heap(C& Data, uint64 Count) const
			{
				uint64 Root = Count / 2 - 1;
				while (true)
				{
					Swap(Data, Count, Root);

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

			template<typename C>
			void Swap(C& Data, uint64 Count, uint64 Root) const
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
					Data[Root] = Data[Largest];
					Data[Largest] = Temp;

					Swap(Data, Count, Largest);
				}
			}

			template<typename C>
			void Swap(C& Data, uint64 Index) const
			{
				T Temp = Data[0];
				Data[0] = Data[Index];
				Data[Index] = Temp;
			}

		private:
			CompareFunction<T> Comparison;
		};
	}
}
