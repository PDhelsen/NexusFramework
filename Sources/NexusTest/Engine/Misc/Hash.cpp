#include "Core/NexusTestPch.h"

#include "Misc/Hash.h"

namespace NxTs
{
	const char* TestSmall = "Test";
	const char* TestLong = "This is a test text for testing the Hash function";

	TEST(Hash, Hash32)
	{
		uint32 Hash1 = NxEn::Hash32::Hash(TestSmall, 4, 0);
		uint32 Hash2 = NxEn::Hash32::Hash(TestLong, 49);

		ASSERT_EQ(Hash1, 0xeac53571);
		ASSERT_EQ(Hash2, 0x3ba1b2d6);

		NxEn::Hash32 Test = NxEn::Hash32(10);
		Test.Accumulate(TestSmall, 4);
		Test.Accumulate(TestLong, 49);
		uint32 Hash = Test.Hash();

		NxEn::Hash32 TestChain = NxEn::Hash32(10);
		TestChain.Accumulate(TestSmall, 4).Accumulate(TestLong, 49);
		uint32 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Hash64)
	{
		uint64 Hash1 = NxEn::Hash64::Hash(TestSmall, 4, 0);
		uint64 Hash2 = NxEn::Hash64::Hash(TestLong, 49);

		ASSERT_EQ(Hash1, 0xda83efc38a8922b4);
		ASSERT_EQ(Hash2, 0xc5eeeb390253f739);

		NxEn::Hash64 Test = NxEn::Hash64(10);
		Test.Accumulate(TestSmall, 4);
		Test.Accumulate(TestLong, 49);
		uint64 Hash = Test.Hash();

		NxEn::Hash64 TestChain = NxEn::Hash64(10);
		TestChain.Accumulate(TestSmall, 4).Accumulate(TestLong, 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}
}