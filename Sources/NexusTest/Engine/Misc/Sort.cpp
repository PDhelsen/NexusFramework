#include "Core/NexusTestPch.h"

#include "Misc/Sort.h"

namespace NxTs
{
	TEST(Sort, SortMerge)
	{
		uint64* Data = new uint64[10];
		
		Data[0] = 8;
		Data[1] = 4;
		Data[2] = 7;
		Data[3] = 1;
		Data[4] = 0;
		Data[5] = 5;
		Data[6] = 6;
		Data[7] = 2;
		Data[8] = 9;
		Data[9] = 3;

		NxEn::SortMerge::Sort(Data, 10);
		for (uint64 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Data[Index], Index);
		}

		delete[] Data;
	}
}