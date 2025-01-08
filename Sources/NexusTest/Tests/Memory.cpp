#include "Core/NexusTestPch.h"

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
		MemoryTest* Test = (MemoryTest*)NxEn::Memory::Allocate(sizeof(MemoryTest) * 5);
		ASSERT_NE(Test, nullptr);

		for (int32 Index = 0; Index < 5; Index++)
		{
			Test[Index].Value = Index;
		}

		Test = (MemoryTest*)NxEn::Memory::Reallocate(Test, sizeof(MemoryTest) * 10);
		ASSERT_NE(Test, nullptr);

		for (int32 Index = 0; Index < 5; Index++)
		{
			ASSERT_EQ(Test[Index].Value, Index);
		}

		NxEn::Memory::Free(Test);
	}

	TEST(Memory, ConstructDestruct)
	{
		void* Pointer = NxEn::Memory::Allocate(sizeof(MemoryTest));
		MemoryTest* Test = NxEn::Memory::Construct<MemoryTest>(Pointer);

		ASSERT_EQ(Test->Value, 120);

		NxEn::Memory::Destruct<MemoryTest>(Test);
		NxEn::Memory::Free(Test);
	}

	TEST(Memory, Operator)
	{
		MemoryTest* Test = new MemoryTest();
		ASSERT_NE(Test, nullptr);
		delete Test;
	}

	TEST(Memory, MallocReallocFreeConstructDestruct)
	{
		MemoryTest * Test = NxEn::Memory::Create<MemoryTest>(sizeof(MemoryTest));
	
		ASSERT_EQ(Test->Value, 120);
	
		NxEn::Memory::Destroy<MemoryTest>(Test);
	}

	TEST(Memory, SetCopyMove)
	{
		uint8* Test = (uint8*)NxEn::Memory::Allocate(sizeof(uint8) * 10);
		NxEn::Memory::MemSet(Test, 5, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Test[Index], 5);
		}

		uint8* Copy = (uint8*)NxEn::Memory::Allocate(sizeof(uint8) * 10);
		NxEn::Memory::MemCopy(Test, Copy, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Copy[Index], 5);
		}

		uint8* Move = (uint8*)NxEn::Memory::Allocate(sizeof(uint8) * 10);
		NxEn::Memory::MemMove(Test, Move, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Move[Index], 5);
		}

		NxEn::Memory::Free(Test);
		NxEn::Memory::Free(Copy);
		NxEn::Memory::Free(Move);
	}

	TEST(Memory, Alignement)
	{
		const uint8 Alignement = 8;

		void* Pointer = new uint64();

		uint64 RawAddress = reinterpret_cast<uint64>(Pointer);
		uint64 AlignedAddress = NxEn::Memory::AlignAddress(RawAddress, Alignement);
		if (RawAddress == AlignedAddress)
		{
			AlignedAddress += Alignement;
		}

		void* AlignedPointer = NxEn::Memory::AlignPointer(Pointer, Alignement);
		ASSERT_EQ(reinterpret_cast<uint64>(AlignedPointer), AlignedAddress);

		uint8* MemoryBlock = reinterpret_cast<uint8*>(AlignedPointer);
		uint8 Shift = MemoryBlock[-1];
		ASSERT_EQ(Shift, Alignement);

		void* UnalignedPointer = NxEn::Memory::UnalignPointer(AlignedPointer);
		ASSERT_EQ(reinterpret_cast<uint64>(UnalignedPointer), reinterpret_cast<uint64>(Pointer));
	
		delete Pointer;
	}
}
