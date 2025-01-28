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

	TEST(Memory, MallocReallocFree)
	{
		MemoryTest* Test = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest) * 5);
		ASSERT_NE(Test, nullptr);

		for (int32 Index = 0; Index < 5; Index++)
		{
			Test[Index].Value = Index;
		}

		Test = (MemoryTest*)NxFr::Memory::Reallocate(Test, sizeof(MemoryTest) * 10);
		ASSERT_NE(Test, nullptr);

		for (int32 Index = 0; Index < 5; Index++)
		{
			ASSERT_EQ(Test[Index].Value, Index);
		}

		NxFr::Memory::Free(Test);
	}

	TEST(Memory, ConstructDestruct)
	{
		void* Pointer = NxFr::Memory::Allocate(sizeof(MemoryTest));
		MemoryTest* Test = NxFr::Memory::Construct<MemoryTest>(Pointer);

		ASSERT_EQ(Test->Value, 120);

		NxFr::Memory::Destruct<MemoryTest>(Test);
		NxFr::Memory::Free(Test);
	}

	TEST(Memory, Operator)
	{
		MemoryTest* Test = new MemoryTest();
		ASSERT_NE(Test, nullptr);
		delete Test;
	}

	TEST(Memory, MallocReallocFreeConstructDestruct)
	{
		MemoryTest * Test = NxFr::Memory::Create<MemoryTest>(sizeof(MemoryTest));

		ASSERT_EQ(Test->Value, 120);

		NxFr::Memory::Destroy<MemoryTest>(Test);
	}

	TEST(Memory, SetCopyMove)
	{
		uint8* Test = (uint8*)NxFr::Memory::Allocate(sizeof(uint8) * 10);
		NxFr::Memory::MemSet(Test, 5, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Test[Index], 5);
		}

		uint8* Copy = (uint8*)NxFr::Memory::Allocate(sizeof(uint8) * 10);
		NxFr::Memory::MemCopy(Test, Copy, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Copy[Index], 5);
		}

		uint8* Move = (uint8*)NxFr::Memory::Allocate(sizeof(uint8) * 10);
		NxFr::Memory::MemMove(Test, Move, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Move[Index], 5);
		}

		NxFr::Memory::Free(Test);
		NxFr::Memory::Free(Copy);
		NxFr::Memory::Free(Move);
	}

	TEST(Memory, Alignement)
	{
		const uint8 Alignement = 8;

		void* Pointer = new uint64();

		uint64 RawAddress = reinterpret_cast<uint64>(Pointer);
		uint64 AlignedAddress = NxFr::Memory::AlignAddress(RawAddress, Alignement);
		if (RawAddress == AlignedAddress)
		{
			AlignedAddress += Alignement;
		}

		void* AlignedPointer = NxFr::Memory::AlignPointer(Pointer, Alignement);
		ASSERT_EQ(reinterpret_cast<uint64>(AlignedPointer), AlignedAddress);

		uint8* MemoryBlock = reinterpret_cast<uint8*>(AlignedPointer);
		uint8 Shift = MemoryBlock[-1];
		ASSERT_EQ(Shift, Alignement);

		void* UnalignedPointer = NxFr::Memory::UnalignPointer(AlignedPointer);
		ASSERT_EQ(reinterpret_cast<uint64>(UnalignedPointer), reinterpret_cast<uint64>(Pointer));

		delete Pointer;
	}
}
