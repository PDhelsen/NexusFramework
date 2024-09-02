#include "Core/NexusTestPch.h"

#include "Types/Enum.h"

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

	TEST(Type_Enum, Invert)
	{
		TestFlag None = TestFlag::Null;
		TestFlag All = ~TestFlag::Null;

		ASSERT_EQ((uint8)None, 0);
		ASSERT_EQ((uint8)All, 255);

		TestFlag First = TestFlag::First;
		TestFlag NotFirst = ~TestFlag::First;

		ASSERT_EQ((uint8)First, 1);
		ASSERT_EQ((uint8)NotFirst, 254);
	}

	TEST(Type_Enum, Or)
	{
		TestFlag Test1 = TestFlag::First | TestFlag::Fourth;
		ASSERT_EQ((uint8)Test1, 0b1001);

		TestFlag Test2 = TestFlag::First | TestFlag::Fourth;
		Test2 |= TestFlag::Third;
		ASSERT_EQ((uint8)Test2, 0b1101);
	}

	TEST(Type_Enum, Xor)
	{
		TestFlag Mask1 = TestFlag::First | TestFlag::Fourth;
		TestFlag Mask2 = TestFlag::First | TestFlag::Third;
		TestFlag Test1 = Mask1 ^ Mask2;
		ASSERT_EQ((uint8)Test1, 0b1100);

		TestFlag Test2 = TestFlag::First | TestFlag::Fourth;
		TestFlag Mask4 = TestFlag::First | TestFlag::Third;
		Test2 ^= Mask4;
		ASSERT_EQ((uint8)Test2, 0b1100);
	}

	TEST(Type_Enum, And)
	{
		TestFlag Test1 = TestFlag::First & TestFlag::Second;
		ASSERT_EQ((uint8)Test1, 0);

		TestFlag Mask1 = TestFlag::First | TestFlag::Second;
		TestFlag Mask2 = TestFlag::Fourth | TestFlag::Second;
		TestFlag Test2 = Mask1 & Mask2;
		ASSERT_EQ((uint8)Test2, 0b10);

		TestFlag Mask3 = TestFlag::First | TestFlag::Third;
		TestFlag Test3 = ~TestFlag::Null;
		Test3 &= Mask3;
		ASSERT_EQ((uint8)Test3, 0b101);
	}

	TEST(Type_Enum, CheckSet)
	{
		TestFlag Test1 = TestFlag::First | TestFlag::Third;
		ASSERT_EQ(Enum::CheckFlag(Test1, TestFlag::Third), true);
		ASSERT_EQ(Enum::CheckFlag(Test1, TestFlag::Fourth), false);

		TestFlag Test2 = TestFlag::First;
		Test2 = Enum::SetFlag(Test2, TestFlag::Third, true);
		ASSERT_EQ((uint8)Test2, 0b101);
		Test2 = Enum::SetFlag(Test2, TestFlag::First, false);
		ASSERT_EQ((uint8)Test2, 0b100);
	}
}
