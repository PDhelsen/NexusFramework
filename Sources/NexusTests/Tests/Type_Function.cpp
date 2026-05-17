#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

namespace NxTs
{
	TEST(Type_Function, Delegate)
	{
		Dummy Data;
		const Dummy ConstData;

		NxFr::Delegate<uint64(uint64)> Function(&Dummy::DummyFunction2);
		uint64 Result1 = Function(5);
		ASSERT_EQ(Result1, 5);

		NxFr::Delegate<uint64(uint64)> Object(&Data, &Dummy::Function2);
		uint64 Result2 = Object(5);
		ASSERT_EQ(Result2, 5);

		NxFr::Delegate<uint64(uint64)> ConstObject(&ConstData, &Dummy::Function2);
		uint64 ConstResult2 = ConstObject(5);
		ASSERT_EQ(Result2, 5);

		NxFr::Delegate<uint64(uint64, uint64)> Lambda([&](uint64 A, uint64 B)->uint64 { return A + B; });
		uint64 Result3 = Lambda(5, 5);
		ASSERT_EQ(Result3, 10);

		NxFr::Delegate<uint64(uint64)> Functor(Data);
		Data.Key = 5;
		uint64 Result4 = Functor(5);
		ASSERT_EQ(Result4, 10);
		Data.Key = 0;

		NxFr::Delegate<uint64(uint64)> Copy = Object;
		uint64 Result5 = Copy(5);
		ASSERT_EQ(Result5, 5);

		NxFr::Delegate<uint64(uint64)> Move = NxFr::Move(Copy);
		uint64 Result6 = Move(5);
		ASSERT_EQ(Result6, 5);
		ASSERT_EQ(Copy.IsNull(), true);

		NxFr::Delegate<uint64(uint64, uint64)> Big([&Data, ConstData](uint64 A, uint64 B)->uint64 { return A + B + Data.Key + ConstData.Key; });
		Data.Key = 5;
		uint64 Result7 = Big(5, 5);
		ASSERT_EQ(Result7, 15);
	}

	TEST(Type_Function, Event)
	{
		Dummy Data;

		NxFr::Delegate<void(uint64)> Function(&Dummy::DummyFunction1);
		NxFr::Delegate<void(uint64)> Object(&Data, &Dummy::Function1);
		NxFr::Delegate<void(uint64)> Lambda([&](uint64 Value) { Data.Key = Value; });

		NxFr::Event<uint64> Event;
		Event += Function;
		Event += Object;
		Event += Lambda;
		Event(1);
		ASSERT_EQ(Event.GetCount(), 3);

		Event -= Function;
		Event -= Object;
		ASSERT_EQ(Event.GetCount(), 1);
	}
}
