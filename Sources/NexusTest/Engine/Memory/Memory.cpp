#include "Core/NexusTestPch.h"

#include "Memory/Memory.h"

namespace NxTs
{
	class MemoryTest
	{
	public:
		MemoryTest()
		{
			Value = 120;
		}

		MemoryTest(int32 V)
		{
			Value = V;
		}

		MemoryTest(const MemoryTest& Other)
		{
			Value = Other.Value;
		}

		~MemoryTest()
		{
			Value = -1;
		}

		int32 Value;
	};

	TEST(Memory, MallocReallocFree)
	{
		MemoryTest* Test = (MemoryTest*)NxEn::Memory::Malloc(sizeof(MemoryTest) * 5);
		ASSERT_NE(Test, nullptr);

		for (int32 Index = 0; Index < 5; Index++)
		{
			Test[Index].Value = Index;
		}

		Test = (MemoryTest*)NxEn::Memory::Realloc(Test, sizeof(MemoryTest) * 10);
		ASSERT_NE(Test, nullptr);

		for (int32 Index = 0; Index < 5; Index++)
		{
			ASSERT_EQ(Test[Index].Value, Index);
		}

		NxEn::Memory::Free(Test);
	}

	TEST(Memory, ConstructDestruct)
	{
		void* Pointer = NxEn::Memory::Malloc(sizeof(MemoryTest));
		MemoryTest* Test = NxEn::Memory::Construct<MemoryTest>(Pointer);

		ASSERT_EQ(Test->Value, 120);

		NxEn::Memory::Destruct<MemoryTest>(Test);
		NxEn::Memory::Free(Test);
	}


	TEST(Memory, AllocateDeallocate)
	{
		MemoryTest* Test = NxEn::Memory::Allocate<MemoryTest>();
		ASSERT_EQ(Test->Value, 120);
		NxEn::Memory::Deallocate<MemoryTest>(Test);

		MemoryTest* TestArray = NxEn::Memory::AllocateArray<MemoryTest>(uint32(10));
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(TestArray[Index].Value, 120);
		}
		NxEn::Memory::DeallocateArray<MemoryTest>(TestArray, 10);
	}
}