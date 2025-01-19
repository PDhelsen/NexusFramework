#include "Core/NexusTests.h"

namespace NxTs
{
	enum class TestFlag : uint8
	{
		Null = 0,
		First = 1 << 0,
		Second = 1 << 1,
		Third = 1 << 2,
		Fourth = 1 << 3,
	};
	NEXUS_ENUM_TO_FLAG(TestFlag)

	TEST(Integer, BitCheckSet)
	{
		uint8 Test = 0b10101010;
		ASSERT_EQ(NxFr::Integer::CheckBit(Test, uint8(1)), true);
		ASSERT_EQ(NxFr::Integer::CheckBit(Test, uint8(0)), false);

		Test = NxFr::Integer::SetBit(Test, uint8(4), true);
		ASSERT_EQ(Test, 0b10111010);
		Test = NxFr::Integer::SetBit(Test, uint8(3), false);
		ASSERT_EQ(Test, 0b10110010);
	}

	TEST(Integer, Enum)
	{
		TestFlag None = TestFlag::Null;
		TestFlag All = ~TestFlag::Null;
		ASSERT_EQ((uint8)None, 0);
		ASSERT_EQ((uint8)All, 255);
		TestFlag First = TestFlag::First;
		TestFlag NotFirst = ~TestFlag::First;
		ASSERT_EQ((uint8)First, 1);
		ASSERT_EQ((uint8)NotFirst, 254);

		TestFlag Test1 = TestFlag::First | TestFlag::Fourth;
		ASSERT_EQ((uint8)Test1, 0b1001);
		TestFlag Test2 = TestFlag::First | TestFlag::Fourth;
		Test2 |= TestFlag::Third;
		ASSERT_EQ((uint8)Test2, 0b1101);

		TestFlag Mask1 = TestFlag::First | TestFlag::Fourth;
		TestFlag Mask2 = TestFlag::First | TestFlag::Third;
		TestFlag Test3 = Mask1 ^ Mask2;
		ASSERT_EQ((uint8)Test3, 0b1100);
		TestFlag Test4 = TestFlag::First | TestFlag::Fourth;
		TestFlag Mask3 = TestFlag::First | TestFlag::Third;
		Test4 ^= Mask3;
		ASSERT_EQ((uint8)Test4, 0b1100);

		TestFlag Test5 = TestFlag::First & TestFlag::Second;
		ASSERT_EQ((uint8)Test5, 0);
		TestFlag Mask4 = TestFlag::First | TestFlag::Second;
		TestFlag Mask5 = TestFlag::Fourth | TestFlag::Second;
		TestFlag Test6 = Mask4 & Mask5;
		ASSERT_EQ((uint8)Test6, 0b10);
		TestFlag Mask7 = TestFlag::First | TestFlag::Third;
		TestFlag Test7 = ~TestFlag::Null;
		Test7 &= Mask7;
		ASSERT_EQ((uint8)Test7, 0b101);

		TestFlag Test8 = TestFlag::First | TestFlag::Third;
		ASSERT_EQ(Enum::CheckFlag(Test8, TestFlag::Third), true);
		ASSERT_EQ(Enum::CheckFlag(Test8, TestFlag::Fourth), false);
		TestFlag Test9 = TestFlag::First;
		Test9 = Enum::SetFlag(Test9, TestFlag::Third, true);
		ASSERT_EQ((uint8)Test9, 0b101);
		Test9 = Enum::SetFlag(Test9, TestFlag::First, false);
		ASSERT_EQ((uint8)Test9, 0b100);
	}
}
