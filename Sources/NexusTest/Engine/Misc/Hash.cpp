#include "Core/NexusTestPch.h"

#include "Misc/Hash.h"

namespace NxTs
{
	struct HashTest
	{
		uint64 A;
		uint64 B;
		uint64 C;
	};
}

namespace NxEn
{
	template<class H>
	struct Hasher<NxTs::HashTest, H>
	{
		static H::HashLength Hash(const NxTs::HashTest& Data, H::HashLength Seed = 0)
		{
			H Algo = H(Seed);
			Algo.Accumulate(&Data.A, sizeof(uint64))
				.Accumulate(&Data.B, sizeof(uint64))
				.Accumulate(&Data.C, sizeof(uint64));
			return Algo.Hash();
		}
	};
}

namespace NxTs
{
	const char* TestSmall = "Test";
	const char* TestLong = "This is a test text for testing the Hash function";
	uint32 Hash32Small = 0xeac53571;
	uint32 Hash32Long = 0x3ba1b2d6;
	uint64 Hash64Small = 0xda83efc38a8922b4;
	uint64 Hash64Long = 0xc5eeeb390253f739;

	TEST(Hash, Hasher)
	{
		uint64 Hash1 = NxEn::Hasher<>::Hash(TestSmall, 4);
		uint64 Hash2 = NxEn::Hasher<>::Hash(TestLong, 49);

		ASSERT_EQ(Hash1, Hash64Small);
		ASSERT_EQ(Hash2, Hash64Long);

		uint64 Test3 = 123456789;
		uint64 Hash3 = NxEn::Hasher<uint64>::Hash(123456789);

		HashTest Test4 = { .A = 10, .B = 15, .C = 20 };
		uint64 Hash4 = NxEn::Hasher<HashTest>::Hash(Test4);

		uint64 Hash5 = NxEn::Hasher<>::Combine(Hash1, Hash2);
	}

	TEST(Hash, Hash32)
	{
		uint32 Hash1 = NxEn::Hash32::Hash(TestSmall, 4);
		uint32 Hash2 = NxEn::Hash32::Hash(TestLong, 49);

		ASSERT_EQ(Hash1, Hash32Small);
		ASSERT_EQ(Hash2, Hash32Long);

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
		uint64 Hash1 = NxEn::Hash64::Hash(TestSmall, 4);
		uint64 Hash2 = NxEn::Hash64::Hash(TestLong, 49);

		ASSERT_EQ(Hash1, Hash64Small);
		ASSERT_EQ(Hash2, Hash64Long);

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