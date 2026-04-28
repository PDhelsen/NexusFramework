#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	enum class TestEnum : uint8
	{
		Null,
		First,
		Second,
		Third,
		Fourth,
	};


	enum class TestFlag : uint8
	{
		None = 0,

		First = 1 << 0,
		Second = 1 << 1,
		Third = 1 << 2,
		Fourth = 1 << 3,
	};
}

NEXUS_ENUM_STRING(NxTs::TestEnum, 5, "Null", "First", "Second", "Third", "Fourth")
NEXUS_FLAG(NxTs::TestFlag, uint8)
NEXUS_FLAG_STRING(NxTs::TestFlag, 4, "First", "Second", "Third", "Fourth")

namespace NxTs
{
	TEST(Type_Numeric, Integer)
	{
		uint8 Bits = 0b10101010;
		ASSERT_EQ(NxFr::Integer::CheckBit(Bits, uint8(1)), true);
		ASSERT_EQ(NxFr::Integer::CheckBit(Bits, uint8(0)), false);
		Bits = NxFr::Integer::SetBit(Bits, uint8(4), true);
		ASSERT_EQ(Bits, 0b10111010);
		Bits = NxFr::Integer::SetBit(Bits, uint8(3), false);
		ASSERT_EQ(Bits, 0b10110010);

		uint8 Flags = 0b10101010;
		ASSERT_EQ(NxFr::Integer::CheckFlag(Flags, uint8(2)), true);
		ASSERT_EQ(NxFr::Integer::CheckFlag(Flags, uint8(4)), false);
		Flags = NxFr::Integer::SetFlag(Flags, uint8(16), true);
		ASSERT_EQ(Flags, 0b10111010);
		Flags = NxFr::Integer::SetFlag(Flags, uint8(8), false);
		ASSERT_EQ(Flags, 0b10110010);
	}

	TEST(Type_Numeric, Enum)
	{
		ASSERT_EQ(NxFr::StringUtility::ToString(TestEnum::Third), "Third");
		ASSERT_EQ(NxFr::StringUtility::FromString<TestEnum>("Third"), TestEnum::Third);

		TestFlag None = TestFlag::None;
		TestFlag All = ~TestFlag::None;
		ASSERT_EQ((uint8)None, 0);
		ASSERT_EQ((uint8)All, 255);
		TestFlag Flag = TestFlag::First;
		ASSERT_EQ((uint8)Flag, 1);
		Flag = ~TestFlag::First;
		ASSERT_EQ((uint8)Flag, 254);
		Flag = TestFlag::First | TestFlag::Fourth;
		Flag |= TestFlag::Third;
		ASSERT_EQ((uint8)Flag, 0b1101);
		TestFlag Mask1 = TestFlag::First | TestFlag::Fourth;
		TestFlag Mask2 = TestFlag::First | TestFlag::Third;
		Flag = Mask1 ^ Mask2;
		ASSERT_EQ((uint8)Flag, 0b1100);
		Flag = TestFlag::First & TestFlag::Second;
		ASSERT_EQ((uint8)Flag, 0);
		Mask1 = TestFlag::First | TestFlag::Second;
		Mask2 = TestFlag::Fourth | TestFlag::Second;
		Flag = Mask1 & Mask2;
		ASSERT_EQ((uint8)Flag, 0b10);
		Mask1 = TestFlag::First | TestFlag::Third;
		Flag = ~TestFlag::None;
		Flag &= Mask1;
		ASSERT_EQ((uint8)Flag, 0b101);

		Flag = TestFlag::First | TestFlag::Third;
		ASSERT_EQ(NxFr::Enum::CheckFlag(Flag, TestFlag::Third), true);
		ASSERT_EQ(NxFr::Enum::CheckFlag(Flag, TestFlag::Fourth), false);
		Flag = TestFlag::First;
		Flag = NxFr::Enum::SetFlag(Flag, TestFlag::Third, true);
		ASSERT_EQ((uint8)Flag, 0b101);
		Flag = NxFr::Enum::SetFlag(Flag, TestFlag::First, false);
		ASSERT_EQ((uint8)Flag, 0b100);
	}
}
