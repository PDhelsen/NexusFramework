#include "Core/NexusTestPch.h"

#include "Types/Numbers/Integer.h"

namespace NxTs
{
	TEST(Type_Integer, BitCheckSet)
	{
		uint8 Test = 0b10101010;
		ASSERT_EQ(NxEn::Integer::CheckBit(Test, uint8(1)), true);
		ASSERT_EQ(NxEn::Integer::CheckBit(Test, uint8(0)), false);

		Test = NxEn::Integer::SetBit(Test, uint8(4), true);
		ASSERT_EQ(Test, 0b10111010);
		Test = NxEn::Integer::SetBit(Test, uint8(3), false);
		ASSERT_EQ(Test, 0b10110010);
	}
}
