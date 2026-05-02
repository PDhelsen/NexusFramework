#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	static uint64 Total = 0;

	void AddTotal(uint64 Value)
	{
		Total += Value;
	}

	int8 Add(int8 A, int8 B)
	{
		return A + B;
	}

	struct DelegateTest
	{
		int8 A;

		int64 Padding[3];

		int8 Add(int8 B, int8 C) const
		{
			return A + B + C;
		}

		void Sum(uint64 Value)
		{
			AddTotal(Value);
		}

		int8 operator()(int8 B, int8 C)
		{
			return Add(B, C);
		}

		void operator()(uint64 Value)
		{
			Sum(Value);
		}
	};

	TEST(Type_Function, Delegate)
	{
		DelegateTest Data { .A = 5 };
		const DelegateTest ConstData{ .A = 5 };

		NxFr::Delegate<int8(int8, int8)> Function(&Add);
		int8 Result1 = Function(4, 5);
		ASSERT_EQ(Result1, 9);

		NxFr::Delegate<int8(int8, int8)> Object(&Data, &DelegateTest::Add);
		int8 Result2 = Object(6, 0);
		ASSERT_EQ(Result2, 11);

		NxFr::Delegate<int8(int8, int8)> ConstObject(&ConstData, &DelegateTest::Add);
		int8 ConstResult2 = ConstObject(6, 0);
		ASSERT_EQ(Result2, 11);

		NxFr::Delegate<int8(int8, int8)> Lambda([&](int8 A, int8 B)->int8 { return A + B; });
		int8 Result3 = Lambda(6, 6);
		ASSERT_EQ(Result3, 12);

		NxFr::Delegate<int8(int8, int8)> Functor(Data);
		Data.A = 10;
		int8 Result4 = Functor(6, 0);
		ASSERT_EQ(Result4, 16);

		NxFr::Delegate<int8(int8, int8)> Copy = Object;
		int8 Result5 = Copy(10, 0);
		ASSERT_EQ(Result5, 20);

		NxFr::Delegate<int8(int8, int8)> Move = NxFr::Move(Copy);
		int8 Result6 = Move(20, 0);
		ASSERT_EQ(Result6, 30);
		ASSERT_EQ(Copy.IsNull(), true);

		Copy = Move;
		Result5 = Copy(10, 0);
		ASSERT_EQ(Result5, 20);

		Move = NxFr::Move(Copy);
		Result6 = Move(20, 0);
		ASSERT_EQ(Result6, 30);
		ASSERT_EQ(Copy.IsNull(), true);

		NxFr::Delegate<int8(int8, int8)> Big([=](int8 A, int8 B)->int8 { return A + B + Data.A; });
		Data.A = 1;
		int8 Result7 = Big(6, 6);
		ASSERT_EQ(Result7, 22);

		NxFr::Delegate<int8(int8, int8)> BigCopy = NxFr::Delegate<int8(int8, int8)>([=](int8 A, int8 B)->int8 { return A + B + Data.A + Data.A; });
		Big = BigCopy;
		int8 Result8 = Big(7, 7);
		ASSERT_EQ(Result8, 16);

		NxFr::Delegate<int8(int8, int8)> CopyOperator = Function;
		ASSERT_EQ(CopyOperator == Function, true);
		ASSERT_EQ(CopyOperator == Object, false);
		ASSERT_EQ(CopyOperator == Functor, false);
		CopyOperator = Object;
		ASSERT_EQ(CopyOperator == Function, false);
		ASSERT_EQ(CopyOperator == Object, true);
		ASSERT_EQ(CopyOperator == Functor, false);
		CopyOperator = Functor;
		ASSERT_EQ(CopyOperator == Function, false);
		ASSERT_EQ(CopyOperator == Object, false);
		ASSERT_EQ(CopyOperator == Functor, true);

		bool IsNull = true;
		if (Function)
		{
			IsNull = false;
		}
		ASSERT_EQ(IsNull, false);
	}

	TEST(Type_Function, Event)
	{
		DelegateTest Data{ .A = 5 };

		NxFr::Delegate<void(uint64)> Function(&AddTotal);
		NxFr::Delegate<void(uint64)> Object(&Data, &DelegateTest::Sum);
		NxFr::Delegate<void(uint64)> Lambda([&](uint64 Value) { AddTotal(Value); });
		NxFr::Delegate<void(uint64)> Functor(Data);

		NxFr::Event<uint64> Event;
		Event += Function;
		Event += Object;
		Event += Lambda;
		Event += Functor;

		Event.Invoke(1);
		ASSERT_EQ(Total, 4);

		Event -= Function;
		Event -= Object;
		Event -= Functor;

		NxFr::Event<uint64> Copy = Event;
		Copy.Invoke(1);
		ASSERT_EQ(Total, 5);

		Event.Clear();
		Event.Invoke(1);
		ASSERT_EQ(Total, 5);

		Event = Copy;
		Event.Invoke(1);
		ASSERT_EQ(Total, 6);

		NxFr::Event<uint64> Temp;
		Temp += &AddTotal;
		Temp += [&](uint64 Value) { AddTotal(Value); };
		Temp += Data;

		Temp.Invoke(1);
		ASSERT_EQ(Total, 9);
	}
}
