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

	TEST(Memory, Allocation)
	{
		void* Pointer;
		MemoryTest* Test;

		Test = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest) * 5);
		ASSERT_NE(Test, nullptr);
		Test = (MemoryTest*)NxFr::Memory::Reallocate(Test, sizeof(MemoryTest) * 10);
		ASSERT_NE(Test, nullptr);
		NxFr::Memory::Free(Test);

		Pointer = NxFr::Memory::Allocate(sizeof(MemoryTest));
		Test = NxFr::Memory::Construct<MemoryTest>(Pointer);
		ASSERT_EQ(Test->Value, 120);
		NxFr::Memory::Destruct<MemoryTest>(Test);
		NxFr::Memory::Free(Test);

		Test = NxFr::Memory::Create<MemoryTest>();
		ASSERT_EQ(Test->Value, 120);
		NxFr::Memory::Destroy<MemoryTest>(Test);

		Test = new MemoryTest();
		ASSERT_NE(Test, nullptr);
		delete Test;
	}

	TEST(Memory, Utilities)
	{
		uint8* Set = (uint8*)NxFr::Memory::Allocate(sizeof(uint8) * 10);
		NxFr::Memory::MemSet(Set, 5, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Set[Index], 5);
		}

		uint8* Copy = (uint8*)NxFr::Memory::Allocate(sizeof(uint8) * 10);
		NxFr::Memory::MemCopy(Set, Copy, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Copy[Index], 5);
		}

		uint8* Move = (uint8*)NxFr::Memory::Allocate(sizeof(uint8) * 10);
		NxFr::Memory::MemMove(Set, Move, 10);
		for (int32 Index = 0; Index < 10; Index++)
		{
			ASSERT_EQ(Move[Index], 5);
		}

		NxFr::Memory::Free(Set);
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

	TEST(Memory, StackAllocator)
	{
		NxFr::StackAllocator* Allocator = new NxFr::StackAllocator(10 * (alignof(MemoryTest) + sizeof(MemoryTest)));
		ASSERT_EQ(Allocator->UsedAmount(), 0);

		MemoryTest* Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 12);

		MemoryTest* Test2 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test2->Value = 2;
		Test2->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 24);

		MemoryTest* Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 36);

		MemoryTest* Test4 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test4->Value = 4;
		Test4->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 48);

		MemoryTest* Test5 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test5->Value = 5;
		Test5->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 60);

		NxFr::Memory::Free(Test3, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 24);

		NxFr::Memory::Free(Test1, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 0);

		Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 12);

		Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator, alignof(MemoryTest));
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 24);

		Allocator->Clear();
		ASSERT_EQ(Allocator->UsedAmount(), 0);

		delete Allocator;
	}

	TEST(Memory, PoolAllocator)
	{
		NxFr::PoolAllocator* Allocator = new NxFr::PoolAllocator(10 * sizeof(MemoryTest), sizeof(MemoryTest));
		ASSERT_EQ(Allocator->UsedAmount(), 0);
		ASSERT_EQ(Allocator->SlotAvailable(), 10);

		MemoryTest* Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 8);
		ASSERT_EQ(Allocator->SlotAvailable(), 9);

		MemoryTest* Test2 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test2->Value = 2;
		Test2->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 16);
		ASSERT_EQ(Allocator->SlotAvailable(), 8);

		MemoryTest* Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 24);
		ASSERT_EQ(Allocator->SlotAvailable(), 7);

		MemoryTest* Test4 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test4->Value = 4;
		Test4->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 32);
		ASSERT_EQ(Allocator->SlotAvailable(), 6);

		MemoryTest* Test5 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test5->Value = 5;
		Test5->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 40);
		ASSERT_EQ(Allocator->SlotAvailable(), 5);

		NxFr::Memory::Free(Test1, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 32);
		ASSERT_EQ(Allocator->SlotAvailable(), 6);

		NxFr::Memory::Free(Test3, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 24);
		ASSERT_EQ(Allocator->SlotAvailable(), 7);

		Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 32);

		Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 40);

		Allocator->Clear();
		ASSERT_EQ(Allocator->UsedAmount(), 0);
		ASSERT_EQ(Allocator->SlotAvailable(), 10);

		delete Allocator;
	}

	TEST(Memory, HeapAllocator)
	{
		NxFr::HeapAllocator* Allocator = new NxFr::HeapAllocator(10 * (16 + 16 + sizeof(MemoryTest)));
		ASSERT_EQ(Allocator->UsedAmount(), 16);

		MemoryTest* Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 48);

		MemoryTest* Test2 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test2->Value = 2;
		Test2->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 80);

		MemoryTest* Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 112);

		MemoryTest* Test4 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test4->Value = 4;
		Test4->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 144);

		MemoryTest* Test5 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test5->Value = 5;
		Test5->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 176);

		NxFr::Memory::Free(Test1, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 160);

		NxFr::Memory::Free(Test3, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 144);

		Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 160);

		Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Allocator);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		ASSERT_EQ(Allocator->UsedAmount(), 176);

		Allocator->Clear();
		ASSERT_EQ(Allocator->UsedAmount(), 16);

		delete Allocator;
	}

	TEST(Memory, AllocatorContext)
	{
		NxFr::HeapAllocator* Allocator = new NxFr::HeapAllocator(512);

		{
			NxFr::AllocatorContext Context(Allocator);

			MemoryTest* Test = new MemoryTest();
			Test->Value = 1;
			Test->Test = 0xffffffff;
			ASSERT_EQ(Allocator->UsedAmount(), 48);

			delete Test;
			ASSERT_EQ(Allocator->UsedAmount(), 16);
		}

		delete Allocator;
	}

	TEST(Memory, Handle)
	{
		MemoryTest* Test1 = new MemoryTest();
		MemoryTest* Test2 = new MemoryTest();

		NxFr::HandleManager Manager = NxFr::HandleManager(10);

		NxFr::Handle<MemoryTest> Handle = Manager.AcquireHandle<MemoryTest>(Test1);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Value, &Test1->Value);
		ASSERT_EQ(Handle->Value, Test1->Value);

		Handle->Value = 1;
		Handle->Test = 0xffffffff;
		ASSERT_EQ(Test1->Value, 1);
		ASSERT_EQ(Test1->Test, 0xffffffff);

		Manager.UpdateHandle<MemoryTest>(Handle, Test2);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Value, &Test2->Value);
		ASSERT_EQ(Handle->Value, Test2->Value);

		Handle->Value = 2;
		Handle->Test = 0xffffffff;
		ASSERT_EQ(Test2->Value, 2);
		ASSERT_EQ(Test2->Test, 0xffffffff);

		ASSERT_EQ(Manager.BelongToManager(Handle), true);
		ASSERT_EQ(Manager.BelongToManager(NxFr::Handle<MemoryTest>()), false);

		Manager.ReleaseHandle<MemoryTest>(Handle);
		ASSERT_EQ(Handle.IsValid(), false);

		delete Test1;
		delete Test2;
	}

	TEST(Memory, Defragmentation)
	{
		NxFr::HeapAllocator* Heap = new NxFr::HeapAllocator(512);
		NxFr::HandleManager Manager = NxFr::HandleManager(10);

		Heap->Defragment(&Manager);

		MemoryTest* Test1 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test1->Value = 1;
		Test1->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle1 = Manager.AcquireHandle<MemoryTest>(Test1);

		MemoryTest* Test2 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test2->Value = 2;
		Test2->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle2 = Manager.AcquireHandle<MemoryTest>(Test2);

		MemoryTest* Test3 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test3->Value = 3;
		Test3->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle3 = Manager.AcquireHandle<MemoryTest>(Test3);

		MemoryTest* Test4 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test4->Value = 4;
		Test4->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle4 = Manager.AcquireHandle<MemoryTest>(Test4);

		MemoryTest* Test5 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test5->Value = 5;
		Test5->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle5 = Manager.AcquireHandle<MemoryTest>(Test5);

		MemoryTest* Test6 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test6->Value = 6;
		Test6->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle6 = Manager.AcquireHandle<MemoryTest>(Test6);

		MemoryTest* Test7 = (MemoryTest*)NxFr::Memory::Allocate(sizeof(MemoryTest), Heap);
		Test7->Value = 7;
		Test7->Test = 0xffffffff;
		NxFr::Handle<MemoryTest> Handle7 = Manager.AcquireHandle<MemoryTest>(Test7);

		NxFr::Memory::Free(Manager.ReleaseHandle(Handle2), Heap);
		NxFr::Memory::Free(Manager.ReleaseHandle(Handle4), Heap);
		NxFr::Memory::Free(Manager.ReleaseHandle(Handle5), Heap);

		uint64 StartAmount = Heap->UsedAmount();
		Heap->Defragment(&Manager, 1.0f);
		Heap->Defragment(&Manager);
		Heap->Defragment(&Manager);
		uint64 EndAmount = Heap->UsedAmount();

		ASSERT_EQ(Handle3->Value, 3);
		ASSERT_EQ(Handle6->Value, 6);
		ASSERT_EQ(Handle7->Value, 7);
		ASSERT_EQ(StartAmount > EndAmount, true);

		Manager.ReleaseHandle(Handle1);
		Manager.ReleaseHandle(Handle3);
		Manager.ReleaseHandle(Handle6);
		Manager.ReleaseHandle(Handle7);

		delete Heap;
	}
}
