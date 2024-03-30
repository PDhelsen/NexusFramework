#include "Core/NexusTestPch.h"

#include "Memory/Memory.h"
#include "Memory/MemoryOperatorDeclaration.h"
#include "Memory/Allocator/StackAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"
#include "Memory/Allocator/HeapAllocator.h"
#include "Memory/Handle/Handle.h"
#include "Memory/Handle/HandleManager.h"

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

	TEST(Memory, Operator)
	{
		MemoryTest* Test = new MemoryTest();
		ASSERT_NE(Test, nullptr);
		delete Test;
	}

	TEST(Memory, SetCopy)
	{
		uint8* Test = (uint8*)NxEn::Memory::Malloc(sizeof(uint8) * 10);
		NxEn::Memory::MemSet(Test, 5, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Test[Index], 5);
		}

		uint8* Copy = (uint8*)NxEn::Memory::Malloc(sizeof(uint8) * 10);
		NxEn::Memory::MemCopy(Test, Copy, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Copy[Index], 5);
		}

		NxEn::Memory::Free(Test);
		NxEn::Memory::Free(Copy);
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

	TEST(Memory, StackAllocator)
	{
		NxEn::StackAllocator* Allocator = new NxEn::StackAllocator(512);

		bool* Test1 = (bool*)Allocator->Allocate(sizeof(bool), alignof(bool));
		*Test1 = true;
		ASSERT_EQ(Allocator->FreeAmount(), 510);

		int16* Test2 = (int16*)Allocator->Allocate(sizeof(int16), alignof(int16));
		*Test2 = 8;
		ASSERT_EQ(Allocator->FreeAmount(), 506);

		int32* Test3 = (int32*)Allocator->Allocate(sizeof(int32), alignof(int32));
		*Test3 = 255;
		ASSERT_EQ(Allocator->FreeAmount(), 500);

		Allocator->Free(Test2);
		ASSERT_EQ(Allocator->FreeAmount(), 510);

		int64* Test4 = (int64*)Allocator->Allocate(sizeof(int64), alignof(int64));
		*Test4 = 1024;
		ASSERT_EQ(Allocator->FreeAmount(), 496);

		Allocator->Clear();
		ASSERT_EQ(Allocator->FreeAmount(), 512);

		int32* Test5 = (int32*)Allocator->Allocate(sizeof(int32) * 5, alignof(int32));
		Test5[0] = 5;
		Test5[1] = 4;
		Test5[2] = 3;
		Test5[3] = 2;
		Test5[4] = 1;
		ASSERT_EQ(Allocator->FreeAmount(), 488);

		Allocator->Clear();
		ASSERT_EQ(Allocator->FreeAmount(), 512);

		ASSERT_EQ(Allocator->CanAllocate(256, 16), true);
		ASSERT_EQ(Allocator->CanAllocate(512, 16), false);

		delete Allocator;
	}

	TEST(Memory, PoolAllocator)
	{
		NxEn::PoolAllocator* Allocator = new NxEn::PoolAllocator(16, sizeof(MemoryTest));
	
		MemoryTest* Test1 = (MemoryTest*)Allocator->Allocate();
		Test1->Value = 1;
		Test1->Test = 0xffffffff;

		MemoryTest* Test2 = (MemoryTest*)Allocator->Allocate();
		Test2->Value = 2;
		Test2->Test = 0xffffffff;

		MemoryTest* Test3 = (MemoryTest*)Allocator->Allocate();
		Test3->Value = 3;
		Test3->Test = 0xffffffff;

		MemoryTest* Test4 = (MemoryTest*)Allocator->Allocate();
		Test4->Value = 4;
		Test4->Test = 0xffffffff;

		MemoryTest* Test5 = (MemoryTest*)Allocator->Allocate();
		Test5->Value = 5;
		Test5->Test = 0xffffffff;

		ASSERT_EQ(Allocator->SlotAvailable(), 11);

		ASSERT_EQ(Allocator->UsedAmount(), 40);

		Allocator->Free(Test1);
		Allocator->Free(Test3);

		ASSERT_EQ(Allocator->UsedAmount(), 24);

		MemoryTest* Test6 = (MemoryTest*)Allocator->Allocate();
		Test6->Value = 6;
		Test6->Test = 0xffffffff;

		Allocator->Free(Test2);

		ASSERT_EQ(Allocator->UsedAmount(), 24);

		MemoryTest* Test7 = (MemoryTest*)Allocator->Allocate();
		Test7->Value = 7;
		Test7->Test = 0xffffffff;

		MemoryTest* Test8 = (MemoryTest*)Allocator->Allocate();
		Test8->Value = 8;
		Test8->Test = 0xffffffff;

		MemoryTest* Test9 = (MemoryTest*)Allocator->Allocate();
		Test9->Value = 9;
		Test9->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 48);

		Allocator->Clear();

		ASSERT_EQ(Allocator->UsedAmount(), 0);

		MemoryTest* Test10 = (MemoryTest*)Allocator->Allocate();
		Test10->Value = 10;
		Test10->Test = 0xffffffff;

		delete Allocator;
	}

	TEST(Memory, HeapAllocator)
	{
		NxEn::HeapAllocator* Allocator = new NxEn::HeapAllocator(512);

		ASSERT_EQ(Allocator->UsedAmount(), 16);

		MemoryTest* Test1 = (MemoryTest*)Allocator->Allocate(sizeof(MemoryTest));
		Test1->Value = 1;
		Test1->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 48);

		MemoryTest* Test2 = (MemoryTest*)Allocator->Allocate(2 * sizeof(MemoryTest));
		Test2[0].Value = 2;
		Test2[0].Test = 0xffffffff;
		Test2[1].Value = 2;
		Test2[1].Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 80);

		MemoryTest* Test3 = (MemoryTest*)Allocator->Allocate(sizeof(MemoryTest));
		Test3->Value = 3;
		Test3->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 112);

		Allocator->Free(Test2);

		ASSERT_EQ(Allocator->UsedAmount(), 96);

		Allocator->Free(Test1);

		ASSERT_EQ(Allocator->UsedAmount(), 64);

		MemoryTest* Test4 = (MemoryTest*)Allocator->Allocate(sizeof(MemoryTest));
		Test4->Value = 4;
		Test4->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 96);

		MemoryTest* Test5 = (MemoryTest*)Allocator->Allocate(sizeof(MemoryTest));
		Test5->Value = 5;
		Test5->Test = 0xffffffff;

		ASSERT_EQ(Allocator->UsedAmount(), 112);

		Allocator->Clear();
		
		ASSERT_EQ(Allocator->UsedAmount(), 16);

		MemoryTest* Test6 = (MemoryTest*)Allocator->Allocate(sizeof(MemoryTest));
		Test6->Value = 6;
		Test6->Test = 0xffffffff;
		
		ASSERT_EQ(Allocator->UsedAmount(), 48);

		delete Allocator;
	}

	TEST(Memory, ActiveAllocator)
	{
		NxEn::HeapAllocator* Allocator = new NxEn::HeapAllocator(512);

		{
			NxEn::AllocatorActive Active(Allocator);

			MemoryTest* Test1 = new MemoryTest();
			Test1->Value = 1;
			Test1->Test = 0xffffffff;

			ASSERT_EQ(Allocator->UsedAmount(), 48);

			delete Test1;

			ASSERT_EQ(Allocator->UsedAmount(), 16);
		}

		delete Allocator;
	}

	TEST(Memory, Handle)
	{
		MemoryTest* Test1 = new MemoryTest();
		MemoryTest* Test2 = new MemoryTest();

		NxEn::Handle<MemoryTest> Handle = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test1);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Value, &Test1->Value);

		Handle->Value = 1;
		Handle->Test = 0xffffffff;

		NxEn::HandleManager::GetInstance()->UpdateHandle<MemoryTest>(Handle, Test2);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Value, &Test2->Value);

		Handle->Value = 2;
		Handle->Test = 0xffffffff;

		NxEn::HandleManager::GetInstance()->ReleaseHandle<MemoryTest>(Handle);
		ASSERT_EQ(Handle.IsValid(), false);

		delete Test1;
		delete Test2;
	}

	TEST(Memory, Defragmentation)
	{
		NxEn::HeapAllocator* Heap = new NxEn::HeapAllocator(512);
		
		Heap->Defragment();

		MemoryTest* Test1 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle1 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test1);

		MemoryTest* Test2 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test2->Value = 2;
		Test2->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle2 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test2);

		MemoryTest* Test3 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle3 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test3);

		MemoryTest* Test4 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test4->Value = 4;
		Test4->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle4 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test4);

		MemoryTest* Test5 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test5->Value = 5;
		Test5->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle5 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test5);

		MemoryTest* Test6 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test6->Value = 6;
		Test6->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle6 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test6);

		MemoryTest* Test7 = (MemoryTest*)Heap->Allocate(sizeof(MemoryTest));
		Test7->Value = 7;
		Test7->Test = 0xffffffff;
		NxEn::Handle<MemoryTest> Handle7 = NxEn::HandleManager::GetInstance()->AcquireHandle<MemoryTest>(Test7);

		Heap->Free(NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle2));
		Heap->Free(NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle4));
		Heap->Free(NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle5));

		uint64 StartAmount = Heap->UsedAmount();
		Heap->Defragment();
		Heap->Defragment();
		uint64 EndAmount = Heap->UsedAmount();

		ASSERT_EQ(Handle3->Value, 3);
		ASSERT_EQ(Handle6->Value, 6);
		ASSERT_EQ(Handle7->Value, 7);
		ASSERT_EQ(StartAmount > EndAmount, true);

		NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle1);
		NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle3);
		NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle6);
		NxEn::HandleManager::GetInstance()->ReleaseHandle(Handle7);

		delete Heap;
	}
}