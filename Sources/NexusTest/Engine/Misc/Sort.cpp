#include "Core/NexusTestPch.h"

#include "Misc/Sort.h"

namespace NxTs
{
	bool CompareFunction(const uint64& A, const uint64& B)
	{
		return A >= B;
	}

	TEST(Sort, MergeSort)
	{
		uint64* Data = new uint64[10];
		
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

		NxEn::Sort::MergeSort(Data, 10);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::MergeSort(Data, 10, &CompareFunction);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		delete[] Data;
	}

	TEST(Sort, QuickSort)
	{
		uint64* Data = new uint64[10];

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

		NxEn::Sort::QuickSort(Data, 10);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::QuickSort(Data, 10, &CompareFunction);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		delete[] Data;
	}

	TEST(Sort, HeapSort)
	{
		uint64* Data = new uint64[10];

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

		NxEn::Sort::HeapSort(Data, 10);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::HeapSort<uint64>(Data, 10, &CompareFunction);
		for (uint64 Index = 1; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		delete[] Data;
	}

	TEST(Sort, Heapify)
	{
		uint64* Data = new uint64[10];

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

		NxEn::Sort::Heapify(Data, 10);
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

		delete[] Data;
	}
}