#include "Core/NexusTestPch.h"

#include "Misc/Misc.h"

namespace NxTs
{
	TEST(Misc, IsPowerOf2)
	{
		ASSERT_EQ(NxEn::IsPowerOfTwo(16), true);
		ASSERT_EQ(NxEn::IsPowerOfTwo(15), false);
	}

	TEST(Misc, Log2PowerOf2)
	{
		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(16), 4);
		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(8), 3);

		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(1), 0);
		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(0), -1);
	}

	TEST(Misc, Modulo)
	{
		ASSERT_EQ(NxEn::Modulo(0, 4), 0);
		ASSERT_EQ(NxEn::Modulo(1, 4), 1);
		ASSERT_EQ(NxEn::Modulo(2, 4), 2);
		ASSERT_EQ(NxEn::Modulo(3, 4), 3);
		ASSERT_EQ(NxEn::Modulo(4, 4), 0);
		ASSERT_EQ(NxEn::Modulo(5, 4), 1);
		ASSERT_EQ(NxEn::Modulo(6, 4), 2);

		ASSERT_EQ(NxEn::Modulo(-0, 4), 0);
		ASSERT_EQ(NxEn::Modulo(-1, 4), 3);
		ASSERT_EQ(NxEn::Modulo(-2, 4), 2);
		ASSERT_EQ(NxEn::Modulo(-3, 4), 1);
		ASSERT_EQ(NxEn::Modulo(-4, 4), 0);
		ASSERT_EQ(NxEn::Modulo(-5, 4), 3);
		ASSERT_EQ(NxEn::Modulo(-6, 4), 2);
	}
}