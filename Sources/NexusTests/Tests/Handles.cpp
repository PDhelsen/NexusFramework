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

	TEST(Handle, HandleManager)
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

		Manager.UpdateHandle<MemoryTest>(Handle, Test2);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Value, &Test2->Value);
		ASSERT_EQ(Handle->Value, Test2->Value);

		Handle->Value = 2;
		Handle->Test = 0xffffffff;

		ASSERT_EQ(Manager.BelongToManager(Handle), true);
		ASSERT_EQ(Manager.BelongToManager(NxFr::Handle<MemoryTest>()), false);

		Manager.ReleaseHandle<MemoryTest>(Handle);
		ASSERT_EQ(Handle.IsValid(), false);

		delete Test1;
		delete Test2;
	}

	TEST(Handle, Defragmentation)
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
