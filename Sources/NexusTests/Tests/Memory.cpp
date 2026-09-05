#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

namespace NxTs
{
	TEST(Memory, Allocation)
	{
		void* Pointer;
		Dummy* Test;

		Test = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy) * 5);
		ASSERT_NE(Test, nullptr);
		Test = (Dummy*)NxFr::Memory::Reallocate(Test, sizeof(Dummy) * 10);
		ASSERT_NE(Test, nullptr);
		NxFr::Memory::Free(Test);

		Pointer = NxFr::Memory::Allocate(sizeof(Dummy));
		Test = NxFr::Memory::Construct<Dummy>(Pointer);
		ASSERT_EQ(Test->Key, 0);
		NxFr::Memory::Destruct<Dummy>(Test);
		NxFr::Memory::Free(Test);

		Test = NxFr::Memory::Create<Dummy>();
		ASSERT_EQ(Test->Key, 0);
		NxFr::Memory::Destroy<Dummy>(Test);

		Test = new Dummy();
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
		NxFr::StackAllocator* Allocator = new NxFr::StackAllocator(10 * (alignof(Dummy) + sizeof(Dummy)));
		ASSERT_EQ(Allocator->UsedAmount(), 0);

		Dummy* Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test1->Key = 1;
		ASSERT_EQ(Allocator->UsedAmount(), 56);

		Dummy* Test2 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test2->Key = 2;
		ASSERT_EQ(Allocator->UsedAmount(), 112);

		Dummy* Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test3->Key = 3;
		ASSERT_EQ(Allocator->UsedAmount(), 168);

		Dummy* Test4 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test4->Key = 4;
		ASSERT_EQ(Allocator->UsedAmount(), 224);

		Dummy* Test5 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test5->Key = 5;
		ASSERT_EQ(Allocator->UsedAmount(), 280);

		NxFr::Memory::Free(Test3, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 112);

		NxFr::Memory::Free(Test1, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 0);

		Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test1->Key = 1;
		ASSERT_EQ(Allocator->UsedAmount(), 56);

		Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator, alignof(Dummy));
		Test3->Key = 3;
		ASSERT_EQ(Allocator->UsedAmount(), 112);

		Allocator->Clear();
		ASSERT_EQ(Allocator->UsedAmount(), 0);

		delete Allocator;
	}

	TEST(Memory, PoolAllocator)
	{
		NxFr::PoolAllocator* Allocator = new NxFr::PoolAllocator(10 * sizeof(Dummy), sizeof(Dummy));
		ASSERT_EQ(Allocator->UsedAmount(), 0);
		ASSERT_EQ(Allocator->SlotAvailable(), 10);

		Dummy* Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;
		ASSERT_EQ(Allocator->UsedAmount(), 48);
		ASSERT_EQ(Allocator->SlotAvailable(), 9);

		Dummy* Test2 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test2->Key = 2;
		ASSERT_EQ(Allocator->UsedAmount(), 96);
		ASSERT_EQ(Allocator->SlotAvailable(), 8);

		Dummy* Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;
		ASSERT_EQ(Allocator->UsedAmount(), 144);
		ASSERT_EQ(Allocator->SlotAvailable(), 7);

		Dummy* Test4 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test4->Key = 4;
		ASSERT_EQ(Allocator->UsedAmount(), 192);
		ASSERT_EQ(Allocator->SlotAvailable(), 6);

		Dummy* Test5 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test5->Key = 5;
		ASSERT_EQ(Allocator->UsedAmount(), 240);
		ASSERT_EQ(Allocator->SlotAvailable(), 5);

		NxFr::Memory::Free(Test1, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 192);
		ASSERT_EQ(Allocator->SlotAvailable(), 6);

		NxFr::Memory::Free(Test3, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 144);
		ASSERT_EQ(Allocator->SlotAvailable(), 7);

		Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;
		ASSERT_EQ(Allocator->UsedAmount(), 192);

		Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;
		ASSERT_EQ(Allocator->UsedAmount(), 240);

		Allocator->Clear();
		ASSERT_EQ(Allocator->UsedAmount(), 0);
		ASSERT_EQ(Allocator->SlotAvailable(), 10);

		delete Allocator;
	}

	TEST(Memory, HeapAllocator)
	{
		NxFr::HeapAllocator* Allocator = new NxFr::HeapAllocator(10 * (16 + 16 + sizeof(Dummy)));
		ASSERT_EQ(Allocator->UsedAmount(), 16);

		Dummy* Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;
		ASSERT_EQ(Allocator->UsedAmount(), 80);

		Dummy* Test2 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test2->Key = 2;
		ASSERT_EQ(Allocator->UsedAmount(), 144);

		Dummy* Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;
		ASSERT_EQ(Allocator->UsedAmount(), 208);

		Dummy* Test4 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test4->Key = 4;
		ASSERT_EQ(Allocator->UsedAmount(), 272);

		Dummy* Test5 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test5->Key = 5;
		ASSERT_EQ(Allocator->UsedAmount(), 336);

		NxFr::Memory::Free(Test1, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 288);

		NxFr::Memory::Free(Test3, Allocator);
		ASSERT_EQ(Allocator->UsedAmount(), 240);

		Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;
		ASSERT_EQ(Allocator->UsedAmount(), 288);

		Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;
		ASSERT_EQ(Allocator->UsedAmount(), 336);

		Allocator->Clear();
		ASSERT_EQ(Allocator->UsedAmount(), 16);

		delete Allocator;
	}

	TEST(Memory, ContinuousAllocator)
	{
		NxFr::ContinuousAllocator* Allocator = new NxFr::ContinuousAllocator(3 * (16 + 16 + sizeof(Dummy)), NxFr::ContinuousAllocator::DefaultCreator<NxFr::HeapAllocator>());
		ASSERT_EQ(Allocator->GetCount(), 0);

		Dummy* Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;
		ASSERT_EQ(Allocator->GetCount(), 1);

		Dummy* Test2 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test2->Key = 2;
		ASSERT_EQ(Allocator->GetCount(), 1);

		Dummy* Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;
		ASSERT_EQ(Allocator->GetCount(), 1);

		Dummy* Test4 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test4->Key = 4;
		ASSERT_EQ(Allocator->GetCount(), 2);

		Dummy* Test5 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test5->Key = 5;
		ASSERT_EQ(Allocator->GetCount(), 2);

		delete Allocator;
	}

	TEST(Memory, FixedAllocator)
	{
		NxFr::FixedAllocator* Allocator = new NxFr::FixedAllocator(3 * (16 + 16 + sizeof(Dummy)));

		Dummy* Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;

		Dummy* Test2 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test2->Key = 2;

		Dummy* Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;

		Dummy* Test4 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test4->Key = 4;

		Dummy* Test5 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test5->Key = 5;

		delete Allocator;
	}

	TEST(Memory, ManagedAllocator)
	{
		NxFr::ManagedAllocator* Allocator = new NxFr::ManagedAllocator(512, 5);

		Allocator->Defragment();

		Dummy* Test1 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test1->Key = 1;
		NxFr::Handle<Dummy> Handle1 = Allocator->Acquire<Dummy>(Test1);

		Dummy* Test2 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test2->Key = 2;
		NxFr::Handle<Dummy> Handle2 = Allocator->Acquire<Dummy>(Test2);

		Dummy* Test3 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test3->Key = 3;
		NxFr::Handle<Dummy> Handle3 = Allocator->Acquire<Dummy>(Test3);

		Dummy* Test4 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test4->Key = 4;
		NxFr::Handle<Dummy> Handle4 = Allocator->Acquire<Dummy>(Test4);

		Dummy* Test5 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test5->Key = 5;
		NxFr::Handle<Dummy> Handle5 = Allocator->Acquire<Dummy>(Test5);

		Dummy* Test6 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test6->Key = 6;
		NxFr::Handle<Dummy> Handle6 = Allocator->Acquire<Dummy>(Test6);

		Dummy* Test7 = (Dummy*)NxFr::Memory::Allocate(sizeof(Dummy), Allocator);
		Test7->Key = 7;
		NxFr::Handle<Dummy> Handle7 = Allocator->Acquire<Dummy>(Test7);

		NxFr::Memory::Free(Allocator->Release(Handle2), Allocator);
		NxFr::Memory::Free(Allocator->Release(Handle4), Allocator);
		NxFr::Memory::Free(Allocator->Release(Handle5), Allocator);

		uint64 StartAmount = Allocator->UsedAmount();
		Allocator->Defragment(1.0f);
		Allocator->Defragment();
		Allocator->Defragment();
		uint64 EndAmount = Allocator->UsedAmount();

		ASSERT_EQ(Handle3->Key, 3);
		ASSERT_EQ(Handle6->Key, 6);
		ASSERT_EQ(Handle7->Key, 7);
		ASSERT_EQ(StartAmount > EndAmount, true);

		Allocator->Release(Handle1);
		Allocator->Release(Handle3);
		Allocator->Release(Handle6);
		Allocator->Release(Handle7);

		delete Allocator;
	}

	TEST(Memory, AllocatorContext)
	{
		NxFr::HeapAllocator* Allocator = new NxFr::HeapAllocator(512);

		{
			NxFr::Allocator::Scope _ = Allocator;
			ASSERT_EQ(NxFr::Allocator::GetContexts().TryGet(), Allocator);

			Dummy* Test = new Dummy();
			Test->Key = 1;
			ASSERT_EQ(Allocator->UsedAmount(), 80);

			delete Test;
			ASSERT_EQ(Allocator->UsedAmount(), 16);
		}

		delete Allocator;
	}

	TEST(Memory, Handle)
	{
		Dummy* Test1 = new Dummy();
		Dummy* Test2 = new Dummy();

		NxFr::HandleManager Bucket = NxFr::HandleManager(10);

		NxFr::Handle<Dummy> Handle = Bucket.Acquire<Dummy>(Test1);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Key, &Test1->Key);
		ASSERT_EQ(Handle->Key, Test1->Key);

		Handle->Key = 1;
		ASSERT_EQ(Test1->Key, 1);

		Bucket.Update<Dummy>(Handle, Test2);
		ASSERT_EQ(Handle.IsValid(), true);
		ASSERT_EQ(&Handle->Key, &Test2->Key);
		ASSERT_EQ(Handle->Key, Test2->Key);

		Handle->Key = 2;
		ASSERT_EQ(Test2->Key, 2);

		ASSERT_EQ(Bucket.Belong(Handle), true);
		ASSERT_EQ(Bucket.Belong(NxFr::Handle<Dummy>()), false);

		Bucket.Release<Dummy>(Handle);
		ASSERT_EQ(Handle.IsValid(), false);

		delete Test1;
		delete Test2;
	}
}
