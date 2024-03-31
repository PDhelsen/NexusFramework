#include "Core/NexusTestPch.h"

#include "Types/Integer.h"

namespace NxTs
{
	TEST(Integer, BitCheckSet)
	{
		uint8 Test = 0b10101010;
		ASSERT_EQ(CheckBit(Test, uint8(1)), true);
		ASSERT_EQ(CheckBit(Test, uint8(0)), false);

		Test = SetBit(Test, uint8(4), true);
		ASSERT_EQ(Test, 0b10111010);
		Test = SetBit(Test, uint8(3), false);
		ASSERT_EQ(Test, 0b10110010);
	}
}