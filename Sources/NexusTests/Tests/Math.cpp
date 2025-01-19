#include "Core/NexusTests.h"

namespace NxTs
{
	TEST(Math, Modulo)
	{
		ASSERT_EQ(NxFr::Math::Modulo(0, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(1, 4), 1);
		ASSERT_EQ(NxFr::Math::Modulo(2, 4), 2);
		ASSERT_EQ(NxFr::Math::Modulo(3, 4), 3);
		ASSERT_EQ(NxFr::Math::Modulo(4, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(5, 4), 1);
		ASSERT_EQ(NxFr::Math::Modulo(6, 4), 2);

		ASSERT_EQ(NxFr::Math::Modulo(-0, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(-1, 4), 3);
		ASSERT_EQ(NxFr::Math::Modulo(-2, 4), 2);
		ASSERT_EQ(NxFr::Math::Modulo(-3, 4), 1);
		ASSERT_EQ(NxFr::Math::Modulo(-4, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(-5, 4), 3);
		ASSERT_EQ(NxFr::Math::Modulo(-6, 4), 2);
	}

	TEST(Math, PowerOfTwo)
	{
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo((uint32)16), true);
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo((uint32)15), false);

		ASSERT_EQ(NxFr::Math::NextPowerOfTwo((uint32)5), 8);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo((uint32)48), 64);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo((uint32)4), 4);

		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)16), 4);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)8), 3);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)1), 0);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)0), -1);
	}

	TEST(Math, Prime)
	{
		ASSERT_EQ(NxFr::Math::IsPrime((uint32)99), false);
		ASSERT_EQ(NxFr::Math::IsPrime((uint32)97), true);

		ASSERT_EQ(NxFr::Math::NextPrime((uint32)74), 79);
		ASSERT_EQ(NxFr::Math::NextPrime((uint32)84), 89);
	}
}
