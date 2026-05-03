#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

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
		ASSERT_EQ(NxFr::StringUtility::ToString(DummyEnum::Third), "Third");
		ASSERT_EQ(NxFr::StringUtility::FromString<DummyEnum>("Third"), DummyEnum::Third);

		DummyFlag None = DummyFlag::None;
		DummyFlag All = ~DummyFlag::None;
		ASSERT_EQ((uint8)None, 0);
		ASSERT_EQ((uint8)All, 255);
		DummyFlag Flag = DummyFlag::First;
		ASSERT_EQ((uint8)Flag, 1);
		Flag = ~DummyFlag::First;
		ASSERT_EQ((uint8)Flag, 254);
		Flag = DummyFlag::First | DummyFlag::Fourth;
		Flag |= DummyFlag::Third;
		ASSERT_EQ((uint8)Flag, 0b1101);
		DummyFlag Mask1 = DummyFlag::First | DummyFlag::Fourth;
		DummyFlag Mask2 = DummyFlag::First | DummyFlag::Third;
		Flag = Mask1 ^ Mask2;
		ASSERT_EQ((uint8)Flag, 0b1100);
		Flag = DummyFlag::First & DummyFlag::Second;
		ASSERT_EQ((uint8)Flag, 0);
		Mask1 = DummyFlag::First | DummyFlag::Second;
		Mask2 = DummyFlag::Fourth | DummyFlag::Second;
		Flag = Mask1 & Mask2;
		ASSERT_EQ((uint8)Flag, 0b10);
		Mask1 = DummyFlag::First | DummyFlag::Third;
		Flag = ~DummyFlag::None;
		Flag &= Mask1;
		ASSERT_EQ((uint8)Flag, 0b101);

		Flag = DummyFlag::First | DummyFlag::Third;
		ASSERT_EQ(NxFr::Enum::CheckFlag(Flag, DummyFlag::Third), true);
		ASSERT_EQ(NxFr::Enum::CheckFlag(Flag, DummyFlag::Fourth), false);
		Flag = DummyFlag::First;
		Flag = NxFr::Enum::SetFlag(Flag, DummyFlag::Third, true);
		ASSERT_EQ((uint8)Flag, 0b101);
		Flag = NxFr::Enum::SetFlag(Flag, DummyFlag::First, false);
		ASSERT_EQ((uint8)Flag, 0b100);
	}
}
