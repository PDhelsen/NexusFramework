#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	class MemoryTest
	{
	public:
		MemoryTest()
		{
			Value = 120;
			Test = 15;
		}

		MemoryTest(const MemoryTest& Other)
		{
			Value = Other.Value;
			Test = Other.Test;
		}

		~MemoryTest()
		{
			Value = -1;
			Test = 0;
		}

		int32 Value;
		int32 Test;
	};

	TEST(Allocator, StackAllocator)
	{
		NxFr::StackAllocator* Allocator = new NxFr::StackAllocator(512);

		bool* Test1 = (bool*)NxFr::Memory::Allocate(sizeof(bool), Allocator, alignof(bool));
		*Test1 = true;
		ASSERT_EQ(Allocator->FreeAmount(), 510);

		int16* Test2 = (int16*)NxFr::Memory::Allocate(sizeof(int16), Allocator, alignof(int16));
		*Test2 = 8;
		ASSERT_EQ(Allocator->FreeAmount(), 506);

		int32* Test3 = (int32*)NxFr::Memory::Allocate(sizeof(int32), Allocator, alignof(int32));
		*Test3 = 255;
		ASSERT_EQ(Allocator->FreeAmount(), 500);

		NxFr::Memory::Free(Test2, Allocator);
		ASSERT_EQ(Allocator->FreeAmount(), 510);

		int64* Test4 = (int64*)NxFr::Memory::Allocate(sizeof(int64), Allocator, alignof(int64));
		*Test4 = 1024;
		ASSERT_EQ(Allocator->FreeAmount(), 496);

		Allocator->Clear();
		ASSERT_EQ(Allocator->FreeAmount(), 512);

		int32* Test5 = (int32*)NxFr::Memory::Allocate(sizeof(int32) * 5, Allocator, alignof(int32));
		Test5[0] = 5;
		Test5[1] = 4;
		Test5[2] = 3;
		Test5[3] = 2;
		Test5[4] = 1;
		ASSERT_EQ(Allocator->FreeAmount(), 488);

		Allocator->Clear();
		ASSERT_EQ(Allocator->FreeAmount(), 512);

		delete Allocator;

		Allocator = new NxFr::StackAllocator(32);

		MemoryTest* Test10 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test10->Value = 1;
		Test10->Test = 0xffffffff;

		MemoryTest* Test11 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test11->Value = 1;
		Test11->Test = 0xffffffff;

		NxFr::Memory::Free(Test11, Allocator);

		MemoryTest* Test12 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test12->Value = 1;
		Test12->Test = 0xffffffff;

		delete Allocator;
	}

	TEST(Allocator, PoolAllocator)
	{
		NxFr::PoolAllocator* Allocator = new NxFr::PoolAllocator(16 * sizeof(MemoryTest), sizeof(MemoryTest));

		MemoryTest* Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;

		MemoryTest* Test2 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test2->Value = 2;
		Test2->Test = 0xffffffff;

		MemoryTest* Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;

		MemoryTest* Test4 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test4->Value = 4;
		Test4->Test = 0xffffffff;

		MemoryTest* Test5 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test5->Value = 5;
		Test5->Test = 0xffffffff;

		ASSERT_EQ(Allocator->SlotAvailable(), 11);

		ASSERT_EQ(Allocator->UsedAmount(), 40);

		NxFr::Memory::Free(Test1, Allocator);
		NxFr::Memory::Free(Test3, Allocator);

		ASSERT_EQ(Allocator->UsedAmount(), 24);

		MemoryTest* Test6 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test6->Value = 6;
		Test6->Test = 0xffffffff;

		NxFr::Memory::Free(Test2, Allocator);

		ASSERT_EQ(Allocator->UsedAmount(), 24);

		MemoryTest* Test7 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test7->Value = 7;
		Test7->Test = 0xffffffff;

		MemoryTest* Test8 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test8->Value = 8;
		Test8->Test = 0xffffffff;

		MemoryTest* Test9 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test9->Value = 9;
		Test9->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 48);

		Allocator->Clear();

		ASSERT_EQ(Allocator->UsedAmount(), 0);
		MemoryTest* Test10 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test10->Value = 10;
		Test10->Test = 0xffffffff;

		delete Allocator;

		Allocator = new NxFr::PoolAllocator(4 * sizeof(MemoryTest), sizeof(MemoryTest));

		MemoryTest* Test15 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test15->Value = 1;
		Test15->Test = 0xffffffff;

		MemoryTest* Test11 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test11->Value = 1;
		Test11->Test = 0xffffffff;

		MemoryTest* Test12 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test12->Value = 1;
		Test12->Test = 0xffffffff;

		MemoryTest* Test13 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test13->Value = 1;
		Test13->Test = 0xffffffff;

		NxFr::Memory::Free(Test13, Allocator);

		MemoryTest* Test14 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test14->Value = 1;
		Test14->Test = 0xffffffff;

		NxFr::Memory::Free(Test11, Allocator);
		NxFr::Memory::Free(Test12, Allocator);

		MemoryTest* Test16 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test16->Value = 1;
		Test16->Test = 0xffffffff;

		delete Allocator;
	}

	TEST(Allocator, HeapAllocator)
	{
		NxFr::HeapAllocator* Allocator = new NxFr::HeapAllocator(512);

		ASSERT_EQ(Allocator->UsedAmount(), 16);

		MemoryTest* Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 48);

		MemoryTest* Test2 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test2[0].Value = 2;
		Test2[0].Test = 0xffffffff;
		Test2[1].Value = 2;
		Test2[1].Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 80);

		MemoryTest* Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 112);

		NxFr::Memory::Free(Test2, Allocator);

		ASSERT_EQ(Allocator->UsedAmount(), 96);

		NxFr::Memory::Free(Test1, Allocator);

		ASSERT_EQ(Allocator->UsedAmount(), 64);

		MemoryTest* Test4 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test4->Value = 4;
		Test4->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 96);

		MemoryTest* Test5 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test5->Value = 5;
		Test5->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 112);

		Allocator->Clear();

		ASSERT_EQ(Allocator->UsedAmount(), 16);

		MemoryTest* Test6 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test6->Value = 6;
		Test6->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 48);

		MemoryTest* Test7 = (MemoryTest*)NxFr::Memory::Reallocate(Test6, sizeof(MemoryTest) * 3, Allocator);
		Test7[1].Value = 7;
		Test7[1].Test = 0xffffffff;
		Test7[2].Value = 8;
		Test7[2].Test = 0xffffffff;

		ASSERT_EQ(Test6, Test7);
		ASSERT_EQ(Test7[0].Value, 6);
		ASSERT_EQ(Allocator->UsedAmount(), 64);

		MemoryTest* Test8 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test8->Value = 9;
		Test8->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 96);

		MemoryTest* Test9 = (MemoryTest*)NxFr::Memory::Reallocate(Test7, sizeof(MemoryTest) * 5, Allocator);
		Test9[3].Value = 10;
		Test9[3].Test = 0xffffffff;
		Test9[4].Value = 11;
		Test9[4].Test = 0xffffffff;

		ASSERT_NE(Test7, Test9);
		ASSERT_EQ(Test9[0].Value, 6);
		ASSERT_EQ(Test9[2].Value, 8);
		ASSERT_EQ(Allocator->UsedAmount(), 128);

		MemoryTest* Test10 = (MemoryTest*)NxFr::Memory::Reallocate(Test9, sizeof(MemoryTest) * 2, Allocator);
		ASSERT_EQ(Test10, Test9);
		ASSERT_EQ(Test10[0].Value, 6);
		ASSERT_EQ(Allocator->UsedAmount(), 112);

		delete Allocator;

		Allocator = new NxFr::HeapAllocator(32 * 4);

		MemoryTest* Test15 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test15->Value = 1;
		Test15->Test = 0xffffffff;

		MemoryTest* Test11 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test11->Value = 1;
		Test11->Test = 0xffffffff;

		MemoryTest* Test12 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test12->Value = 1;
		Test12->Test = 0xffffffff;

		MemoryTest* Test13 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test13->Value = 1;
		Test13->Test = 0xffffffff;

		NxFr::Memory::Free(Test13, Allocator);

		MemoryTest* Test14 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test14->Value = 1;
		Test14->Test = 0xffffffff;

		NxFr::Memory::Free(Test11, Allocator);
		NxFr::Memory::Free(Test12, Allocator);

		MemoryTest* Test16 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test16->Value = 1;
		Test16->Test = 0xffffffff;

		delete Allocator;
	}

	TEST(Allocator, AllocatorContext)
	{
		NxFr::HeapAllocator* Allocator = new NxFr::HeapAllocator(512);

		{
			NxFr::AllocatorContext Context(Allocator);

			MemoryTest* Test1 = new MemoryTest();
			Test1->Value = 1;
			Test1->Test = 0xffffffff;

			ASSERT_EQ(Allocator->UsedAmount(), 48);

			delete Test1;

			ASSERT_EQ(Allocator->UsedAmount(), 16);
		}

		delete Allocator;
	}
}
