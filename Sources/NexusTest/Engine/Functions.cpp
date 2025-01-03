#include "Core/NexusTestPch.h"

namespace NxTs
{
	void LogMessage(const NxEn::String& Message)
	{
		
	}

	int8 Add(int8 A, int8 B)
	{
		return A + B;
	}

	struct DelegateTest
	{
		int8 A;

		int64 Padding[3];

		int8 Add(int8 B, int8 C)
		{
			return A + B + C;
		}

		void Log(const NxEn::String& Message)
		{
			LogMessage(Message);
		}

		int8 operator()(int8 B, int8 C)
		{
			return Add(B, C);
		}

		void operator()(const NxEn::String& Message)
		{
			Log(Message);
		}
	};

	TEST(Functions, Delegate)
	{
		DelegateTest Data { .A = 5 };

		NxEn::Delegate<int8(int8, int8)> Function(&Add);
		int8 Result1 = Function(4, 5);
		ASSERT_EQ(Result1, 9);

		NxEn::Delegate<int8(int8, int8)> Object(&Data, &DelegateTest::Add);
		int8 Result2 = Object(6, 0);
		ASSERT_EQ(Result2, 11);

		NxEn::Delegate<int8(int8, int8)> Lambda([&](int8 A, int8 B)->int8 { return A + B; });
		int8 Result3 = Lambda(6, 6);
		ASSERT_EQ(Result3, 12);

		NxEn::Delegate<int8(int8, int8)> Functor(Data);
		Data.A = 10;
		int8 Result4 = Functor(6, 0);
		ASSERT_EQ(Result4, 16);

		NxEn::Delegate<int8(int8, int8)> Copy = Object;
		int8 Result5 = Copy(10, 0);
		ASSERT_EQ(Result5, 20);

		NxEn::Delegate<int8(int8, int8)> Move = NxEn::Move(Copy);
		int8 Result6 = Move(20, 0);
		ASSERT_EQ(Result6, 30);
		ASSERT_EQ(Copy.IsNull(), true);

		Copy = Move;
		Result5 = Copy(10, 0);
		ASSERT_EQ(Result5, 20);

		Move = NxEn::Move(Copy);
		Result6 = Move(20, 0);
		ASSERT_EQ(Result6, 30);
		ASSERT_EQ(Copy.IsNull(), true);

		NxEn::Delegate<int8(int8, int8)> Big([=](int8 A, int8 B)->int8 { return A + B + Data.A; });
		Data.A = 1;
		int8 Result7 = Big(6, 6);
		ASSERT_EQ(Result7, 22);

		NxEn::Delegate<int8(int8, int8)> BigCopy = NxEn::Delegate<int8(int8, int8)>([=](int8 A, int8 B)->int8 { return A + B + Data.A + Data.A; });
		Big = BigCopy;
		int8 Result8 = Big(7, 7);
		ASSERT_EQ(Result8, 16);

		NxEn::Delegate<int8(int8, int8)> CopyOperator = Function;
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

	TEST(Functions, Event)
	{
		NxEn::String Message = "Call from Event";
		DelegateTest Data{ .A = 5 };

		NxEn::Delegate<void(const NxEn::String&)> Function(&LogMessage);
		NxEn::Delegate<void(const NxEn::String&)> Object(&Data, &DelegateTest::Log);
		NxEn::Delegate<void(const NxEn::String&)> Lambda([&](const NxEn::String& Message) { LogMessage(Message); });
		NxEn::Delegate<void(const NxEn::String&)> Functor(Data);

		NxEn::Event<const NxEn::String&> Event;
		Event += Function;
		Event += Object;
		Event += Lambda;
		Event += Functor;

		Event.Invoke(Message);

		Event -= Function;
		Event -= Object;
		Event -= Functor;

		NxEn::Event<const NxEn::String&> Copy = Event;

		Event.Invoke(Message);

		Event.Clear();

		Event.Invoke(Message);

		Event = Copy;

		Event.Invoke(Message);
	}
}
