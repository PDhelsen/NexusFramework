#include "Core/NexusTests.h"

namespace NxTs
{
	struct HashTest
	{
		uint64 A;
		uint64 B;
		uint64 C;
	};
}

namespace NxFr::Hashing
{
	template<typename H>
	class HashProcess<NxTs::HashTest, H>
	{
	public:
		static void Accumulate(HashStrategy<H>& State, const NxTs::HashTest& Data)
		{
			State.Accumulate(&Data.A, sizeof(uint64))
				.Accumulate(&Data.B, sizeof(uint64))
				.Accumulate(&Data.C, sizeof(uint64));
		}

		static typename H::HashLength Hash(HashStrategy<H>& State, const NxTs::HashTest& Data)
		{
			HashProcess<NxTs::HashTest, H>::Accumulate(State, Data);
			return State.Hash();
		}
	};
}

namespace NxTs
{
	const NxFr::String TestSmall = "Test";
	const NxFr::String TestLong = "This is a test text for testing the Hash function";

	uint32 XxHash32Small = 0xeac53571;
	uint32 XxHash32Long = 0x3ba1b2d6;
	uint64 XxHash64Small = 0xda83efc38a8922b4;
	uint64 XxHash64Long = 0xc5eeeb390253f739;

	uint32 Murmur32Small = 0x07556ca6;
	uint32 Murmur32Long = 0x1da26dd2;

	uint64 Fnv164Small = 0x76001F7E0213B2C9;
	uint64 Fnv164Long = 0x0DB54C1C1610EA87;
	uint64 Fnv1a64Small = 0x2474E7FB1AEC9F05;
	uint64 Fnv1a64Long = 0x77F122B9F752AACB;

	TEST(Hash, XxHash32)
	{
		uint32 Hash1 = NxFr::Hashing::XxHash32().Accumulate(TestSmall.C(), 4).Hash();
		uint32 Hash2 = NxFr::Hashing::XxHash32().Accumulate(TestLong.C(), 49).Hash();

		ASSERT_EQ(Hash1, XxHash32Small);
		ASSERT_EQ(Hash2, XxHash32Long);

		NxFr::Hashing::XxHash32 Test = NxFr::Hashing::XxHash32(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint32 Hash = Test.Hash();

		NxFr::Hashing::XxHash32 TestChain = NxFr::Hashing::XxHash32(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint32 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, XxHash64)
	{
		uint64 Hash1 = NxFr::Hashing::XxHash64().Accumulate(TestSmall.C(), 4).Hash();
		uint64 Hash2 = NxFr::Hashing::XxHash64().Accumulate(TestLong.C(), 49).Hash();

		ASSERT_EQ(Hash1, XxHash64Small);
		ASSERT_EQ(Hash2, XxHash64Long);

		NxFr::Hashing::XxHash64 Test = NxFr::Hashing::XxHash64(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint64 Hash = Test.Hash();

		NxFr::Hashing::XxHash64 TestChain = NxFr::Hashing::XxHash64(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Murmur32)
	{
		uint32 Hash1 = NxFr::Hashing::Murmur32().Accumulate(TestSmall.C(), 4).Hash();
		uint32 Hash2 = NxFr::Hashing::Murmur32().Accumulate(TestLong.C(), 49).Hash();

		ASSERT_EQ(Hash1, Murmur32Small);
		ASSERT_EQ(Hash2, Murmur32Long);

		NxFr::Hashing::Murmur32 Test = NxFr::Hashing::Murmur32(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint32 Hash = Test.Hash();

		NxFr::Hashing::Murmur32 TestChain = NxFr::Hashing::Murmur32(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint32 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Fnv164)
	{
		uint64 Hash1 = NxFr::Hashing::Fnv164().Accumulate(TestSmall.C(), 4).Hash();
		uint64 Hash2 = NxFr::Hashing::Fnv164().Accumulate(TestLong.C(), 49).Hash();

		ASSERT_EQ(Hash1, Fnv164Small);
		ASSERT_EQ(Hash2, Fnv164Long);

		NxFr::Hashing::Fnv164 Test = NxFr::Hashing::Fnv164(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint64 Hash = Test.Hash();

		NxFr::Hashing::Fnv164 TestChain = NxFr::Hashing::Fnv164(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Fnv1a64)
	{
		uint64 Hash1 = NxFr::Hashing::Fnv1a64().Accumulate(TestSmall.C(), 4).Hash();
		uint64 Hash2 = NxFr::Hashing::Fnv1a64().Accumulate(TestLong.C(), 49).Hash();

		ASSERT_EQ(Hash1, Fnv1a64Small);
		ASSERT_EQ(Hash2, Fnv1a64Long);

		NxFr::Hashing::Fnv1a64 Test = NxFr::Hashing::Fnv1a64(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint64 Hash = Test.Hash();

		NxFr::Hashing::Fnv1a64 TestChain = NxFr::Hashing::Fnv1a64(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Hasher)
	{
		NxFr::Hasher<> Test1;
		Test1.Accumulate(TestSmall);
		uint64 Hash1 = Test1.Hash();

		ASSERT_EQ(Hash1, Fnv1a64Small);

		NxFr::Hasher<> Test2;
		Test2.Accumulate(TestLong);
		uint64 Hash2 = Test2.Hash();

		ASSERT_EQ(Hash2, Fnv1a64Long);
	}

	TEST(Hash, Hash)
	{
		uint64 Hash1 = NxFr::Hash<>::HashData(TestSmall.C(), 4);
		uint64 Hash2 = NxFr::Hash<>::HashData(TestLong.C(), 49);

		ASSERT_EQ(Hash1, Fnv1a64Small);
		ASSERT_EQ(Hash2, Fnv1a64Long);

		uint64 Test3 = 123456789;
		uint64 Hash3 = NxFr::Hash<>::HashObject(123456789);
		HashTest Test4 = { .A = 10, .B = 15, .C = 20 };
		uint64 Hash4 = NxFr::Hash<>::HashObject(Test4);

		ASSERT_EQ(Hash3, Test3);

		uint64 Hash5 = NxFr::Hash<>::Combine(Hash1, Hash2);

		uint32 Hash6 = NxFr::Hash<NxFr::Hashing::XxHash32>::HashObject(TestSmall);
		uint32 Hash7 = NxFr::Hash<NxFr::Hashing::XxHash32>::HashObject(TestLong);

		ASSERT_EQ(Hash6, XxHash32Small);
		ASSERT_EQ(Hash7, XxHash32Long);
	}

	TEST(Hash, NativeType)
	{
		ASSERT_EQ(NxFr::Hash<>::HashObject(int8(8)), 8);
		ASSERT_EQ(NxFr::Hash<>::HashObject(int16(16)), 16);
		ASSERT_EQ(NxFr::Hash<>::HashObject(int32(32)), 32);
		ASSERT_EQ(NxFr::Hash<>::HashObject(int64(64)), 64);
		ASSERT_EQ(NxFr::Hash<>::HashObject(uint8(8)), 8);
		ASSERT_EQ(NxFr::Hash<>::HashObject(uint16(16)), 16);
		ASSERT_EQ(NxFr::Hash<>::HashObject(uint32(32)), 32);
		ASSERT_EQ(NxFr::Hash<>::HashObject(uint64(64)), 64);

		ASSERT_NE(NxFr::Hash<>::HashObject(float(1.0f)), 0);
		ASSERT_NE(NxFr::Hash<>::HashObject(double(2.0f)), 0);

		ASSERT_EQ(NxFr::Hash<>::HashObject(bool(1)), 1);
		ASSERT_EQ(NxFr::Hash<>::HashObject(false), 0);

		HashTest Data = { .A = 10, .B = 15, .C = 20 };
		ASSERT_NE(NxFr::Hash<>::HashObject(&Data), 0);
	}
}
