#include "Core/NexusTestPch.h"

#include "Misc/Sort.h"
#include "Types/Containers/Array.h"

namespace NxTs
{
	bool CompareFunction(const uint64& A, const uint64& B)
	{
		return A >= B;
	}

	TEST(Sort, MergeSort)
	{
		NxEn::Array<uint64> Data = NxEn::Array<uint64>(10);
		Data[0] = 5;
		Data[1] = 4;
		Data[2] = 7;
		Data[3] = 1;
		Data[4] = 0;
		Data[5] = 5;
		Data[6] = 6;
		Data[7] = 2;
		Data[8] = 5;
		Data[9] = 3;

		NxEn::List<uint64> Data2 = NxEn::List<uint64>(10);
		Data2.AppendRange(Data);

		NxEn::Dequeue<uint64> Data3 = NxEn::Dequeue<uint64>();
		Data3.AppendBackRange(Data);

		NxEn::Sort::MergeSort<uint64>(Data);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::MergeSort<uint64>(Data, &CompareFunction);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		NxEn::Sort::MergeSort<uint64>(Data2);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data2[Index - 1] <= Data2[Index], true);
		}

		NxEn::Sort::MergeSort<uint64>(Data3);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data3[Index - 1] <= Data3[Index], true);
		}
	}

	TEST(Sort, QuickSort)
	{
		NxEn::Array<uint64> Data = NxEn::Array<uint64>(10);
		Data[0] = 5;
		Data[1] = 4;
		Data[2] = 7;
		Data[3] = 1;
		Data[4] = 0;
		Data[5] = 5;
		Data[6] = 6;
		Data[7] = 2;
		Data[8] = 5;
		Data[9] = 3;

		NxEn::List<uint64> Data2 = NxEn::List<uint64>(10);
		Data2.AppendRange(Data);

		NxEn::Dequeue<uint64> Data3 = NxEn::Dequeue<uint64>();
		Data3.AppendBackRange(Data);

		NxEn::Sort::QuickSort<uint64>(Data);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::QuickSort<uint64>(Data, &CompareFunction);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		NxEn::Sort::QuickSort<uint64>(Data2);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data2[Index - 1] <= Data2[Index], true);
		}

		NxEn::Sort::QuickSort<uint64>(Data3);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data3[Index - 1] <= Data3[Index], true);
		}
	}

	TEST(Sort, HeapSort)
	{
		NxEn::Array<uint64> Data = NxEn::Array<uint64>(10);
		Data[0] = 5;
		Data[1] = 4;
		Data[2] = 7;
		Data[3] = 1;
		Data[4] = 0;
		Data[5] = 5;
		Data[6] = 6;
		Data[7] = 2;
		Data[8] = 5;
		Data[9] = 3;

		NxEn::List<uint64> Data2 = NxEn::List<uint64>(10);
		Data2.AppendRange(Data);

		NxEn::Dequeue<uint64> Data3 = NxEn::Dequeue<uint64>();
		Data3.AppendBackRange(Data);

		NxEn::Sort::HeapSort<uint64>(Data);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::HeapSort<uint64>(Data, &CompareFunction);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		NxEn::Sort::HeapSort<uint64>(Data2);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data2[Index - 1] <= Data2[Index], true);
		}

		NxEn::Sort::HeapSort<uint64>(Data3);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data3[Index - 1] <= Data3[Index], true);
		}
	}

	TEST(Sort, Heapify)
	{
		NxEn::Array<uint64> Data = NxEn::Array<uint64>(10);
		Data[0] = 5;
		Data[1] = 4;
		Data[2] = 7;
		Data[3] = 1;
		Data[4] = 0;
		Data[5] = 5;
		Data[6] = 6;
		Data[7] = 2;
		Data[8] = 5;
		Data[9] = 3;

		NxEn::List<uint64> Data2 = NxEn::List<uint64>(10);
		Data2.AppendRange(Data);

		NxEn::Dequeue<uint64> Data3 = NxEn::Dequeue<uint64>();
		Data3.AppendBackRange(Data);

		NxEn::Sort::Heapify<uint64>(Data);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			uint64 Left = 2 * Index + 1;
			if (Left < 10)
			{
				ASSERT_EQ(Data[Index] >= Data[Left], true);
			}
			
			uint64 Right = 2 * Index + 2;
			if (Right < 10)
			{
				ASSERT_EQ(Data[Index] >= Data[Right], true);
			}
		}

		NxEn::Sort::Heapify<uint64>(Data2);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			uint64 Left = 2 * Index + 1;
			if (Left < 10)
			{
				ASSERT_EQ(Data2[Index] >= Data2[Left], true);
			}

			uint64 Right = 2 * Index + 2;
			if (Right < 10)
			{
				ASSERT_EQ(Data2[Index] >= Data2[Right], true);
			}
		}

		NxEn::Sort::Heapify<uint64>(Data3);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			uint64 Left = 2 * Index + 1;
			if (Left < 10)
			{
				ASSERT_EQ(Data3[Index] >= Data3[Left], true);
			}

			uint64 Right = 2 * Index + 2;
			if (Right < 10)
			{
				ASSERT_EQ(Data3[Index] >= Data3[Right], true);
			}
		}
	}
}