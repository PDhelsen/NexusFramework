#include "Core/NexusTestPch.h"

#include "Misc/Misc.h"

namespace NxTs
{
	TEST(Integer, IsPowerOf2)
	{
		ASSERT_EQ(NxEn::IsPowerOfTwo(16), true);
		ASSERT_EQ(NxEn::IsPowerOfTwo(15), false);
	}

	TEST(Integer, Log2PowerOf2)
	{
		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(16), 4);
		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(8), 3);

		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(1), 0);
		ASSERT_EQ(NxEn::LogTwoPowerOfTwo(0), -1);
	}
}