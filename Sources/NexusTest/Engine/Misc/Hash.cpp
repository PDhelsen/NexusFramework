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
	struct Hash<NxTs::HashTest, H>
	{
		static H::HashLength HashObject(const NxTs::HashTest& Data, H::HashLength Seed = 0)
		{
			H Hashing = H(Seed);
			Hashing.Accumulate(&Data.A, sizeof(uint64))
				.Accumulate(&Data.B, sizeof(uint64))
				.Accumulate(&Data.C, sizeof(uint64));
			return Hashing.Hash();
		}
	};
}

namespace NxTs
{
	const NxEn::String TestSmall = "Test";
	const NxEn::String TestLong = "This is a test text for testing the Hash function";

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
		uint32 Hash1 = NxEn::XxHash32::Hash(TestSmall.C(), 4);
		uint32 Hash2 = NxEn::XxHash32::Hash(TestLong.C(), 49);

		ASSERT_EQ(Hash1, XxHash32Small);
		ASSERT_EQ(Hash2, XxHash32Long);

		NxEn::XxHash32 Test = NxEn::XxHash32(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint32 Hash = Test.Hash();

		NxEn::XxHash32 TestChain = NxEn::XxHash32(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint32 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, XxHash64)
	{
		uint64 Hash1 = NxEn::XxHash64::Hash(TestSmall.C(), 4);
		uint64 Hash2 = NxEn::XxHash64::Hash(TestLong.C(), 49);

		ASSERT_EQ(Hash1, XxHash64Small);
		ASSERT_EQ(Hash2, XxHash64Long);

		NxEn::XxHash64 Test = NxEn::XxHash64(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint64 Hash = Test.Hash();

		NxEn::XxHash64 TestChain = NxEn::XxHash64(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Murmur32)
	{
		uint32 Hash1 = NxEn::Murmur32::Hash(TestSmall.C(), 4);
		uint32 Hash2 = NxEn::Murmur32::Hash(TestLong.C(), 49);

		ASSERT_EQ(Hash1, Murmur32Small);
		ASSERT_EQ(Hash2, Murmur32Long);

		NxEn::Murmur32 Test = NxEn::Murmur32(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint32 Hash = Test.Hash();

		NxEn::Murmur32 TestChain = NxEn::Murmur32(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint32 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Fnv164)
	{
		uint64 Hash1 = NxEn::Fnv164::Hash(TestSmall.C(), 4);
		uint64 Hash2 = NxEn::Fnv164::Hash(TestLong.C(), 49);

		ASSERT_EQ(Hash1, Fnv164Small);
		ASSERT_EQ(Hash2, Fnv164Long);

		NxEn::Fnv164 Test = NxEn::Fnv164(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint64 Hash = Test.Hash();

		NxEn::Fnv164 TestChain = NxEn::Fnv164(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Fnv1a64)
	{
		uint64 Hash1 = NxEn::Fnv1a64::Hash(TestSmall.C(), 4);
		uint64 Hash2 = NxEn::Fnv1a64::Hash(TestLong.C(), 49);

		ASSERT_EQ(Hash1, Fnv1a64Small);
		ASSERT_EQ(Hash2, Fnv1a64Long);

		NxEn::Fnv1a64 Test = NxEn::Fnv1a64(10);
		Test.Accumulate(TestSmall.C(), 4);
		Test.Accumulate(TestLong.C(), 49);
		uint64 Hash = Test.Hash();

		NxEn::Fnv1a64 TestChain = NxEn::Fnv1a64(10);
		TestChain.Accumulate(TestSmall.C(), 4).Accumulate(TestLong.C(), 49);
		uint64 HashChain = Test.Hash();

		ASSERT_EQ(Hash, HashChain);
	}

	TEST(Hash, Hash)
	{
		uint64 Hash1 = NxEn::Hash<>::HashData(TestSmall.C(), 4);
		uint64 Hash2 = NxEn::Hash<>::HashData(TestLong.C(), 49);

		ASSERT_EQ(Hash1, Fnv1a64Small);
		ASSERT_EQ(Hash2, Fnv1a64Long);

		uint64 Test3 = 123456789;
		uint64 Hash3 = NxEn::Hash<uint64>::HashObject(123456789);

		HashTest Test4 = { .A = 10, .B = 15, .C = 20 };
		uint64 Hash4 = NxEn::Hash<HashTest>::HashObject(Test4);

		uint64 Hash5 = NxEn::Hash<>::Combine(Hash1, Hash2);

		uint32 Hash6 = NxEn::Hash<NxEn::String, NxEn::XxHash32>::HashObject(TestSmall);
		uint32 Hash7 = NxEn::Hash<NxEn::String, NxEn::XxHash32>::HashObject(TestLong);

		ASSERT_EQ(Hash6, XxHash32Small);
		ASSERT_EQ(Hash7, XxHash32Long);
	}

	TEST(Hash, NativeType)
	{
		ASSERT_EQ(NxEn::Hash<int8>::HashObject(int8(8)), 8);
		ASSERT_EQ(NxEn::Hash<int16>::HashObject(int16(16)), 16);
		ASSERT_EQ(NxEn::Hash<int32>::HashObject(int32(32)), 32);
		ASSERT_EQ(NxEn::Hash<int64>::HashObject(int64(64)), 64);
		ASSERT_EQ(NxEn::Hash<uint8>::HashObject(uint8(8)), 8);
		ASSERT_EQ(NxEn::Hash<uint16>::HashObject(uint16(16)), 16);
		ASSERT_EQ(NxEn::Hash<uint32>::HashObject(uint32(32)), 32);
		ASSERT_EQ(NxEn::Hash<uint64>::HashObject(uint64(64)), 64);

		ASSERT_NE(NxEn::Hash<float>::HashObject(float(1.0f)), 0);
		ASSERT_NE(NxEn::Hash<double>::HashObject(double(2.0f)), 0);

		ASSERT_EQ(NxEn::Hash<bool>::HashObject(bool(1)), 1);
		ASSERT_EQ(NxEn::Hash<bool>::HashObject(false), 0);

		HashTest Data = { .A = 10, .B = 15, .C = 20 };
		ASSERT_NE(NxEn::Hash<void*>::HashObject(&Data), 0);
	}
}
