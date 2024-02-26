#include "Core/NexusTestPch.h"

#include "Types/Flags.h"

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
	NEXUS_FLAG(TestFlag, uint8)

	TEST(Flags, Invert)
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

	TEST(Flags, Or)
	{
		TestFlag TestOne = TestFlag::First | TestFlag::Fourth;
		ASSERT_EQ((uint8)TestOne, 0b1001);

		TestFlag TestTwo = TestFlag::First | TestFlag::Fourth;
		TestTwo |= TestFlag::Third;
		ASSERT_EQ((uint8)TestTwo, 0b1101);
	}

	TEST(Flags, Xor)
	{
		TestFlag MaskOne = TestFlag::First | TestFlag::Fourth;
		TestFlag MaskTwo = TestFlag::First | TestFlag::Third;
		TestFlag TestOne = MaskOne ^ MaskTwo;
		ASSERT_EQ((uint8)TestOne, 0b1100);

		TestFlag TestTwo = TestFlag::First | TestFlag::Fourth;
		TestFlag MaskFour = TestFlag::First | TestFlag::Third;
		TestTwo ^= MaskFour;
		ASSERT_EQ((uint8)TestTwo, 0b1100);
	}

	TEST(Flags, And)
	{
		TestFlag TestOne = TestFlag::First & TestFlag::Second;
		ASSERT_EQ((uint8)TestOne, 0);

		TestFlag MaskOne = TestFlag::First | TestFlag::Second;
		TestFlag MaskTwo = TestFlag::Fourth | TestFlag::Second;
		TestFlag TestTwo = MaskOne & MaskTwo;
		ASSERT_EQ((uint8)TestTwo, 0b10);

		TestFlag MaskThree = TestFlag::First | TestFlag::Third;
		TestFlag TestThree = ~TestFlag::Null;
		TestThree &= MaskThree;
		ASSERT_EQ((uint8)TestThree, 0b101);
	}
}