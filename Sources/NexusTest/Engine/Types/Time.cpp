#include "Core/NexusTestPch.h"

#include "Time/Timestamp.h"
#include "Time/Timespan.h"
#include "Time/Time.h"

namespace NxTs
{
	TEST(Type_Time, Timespan)
	{
		NxEn::Timespan Span = NxEn::Timespan(1, 10, 5, 23, 55, 30);
		ASSERT_EQ(Span.ToString(), "0001y10m05d - 23h55m30s");

		NxEn::Timespan Test1 = NxEn::Timespan(1, 1, 1, 1, 1, 1);
		NxEn::Timespan Test2 = NxEn::Timespan(1, 2, 2, 2, 2, 2);
		NxEn::Timespan Test3 = NxEn::Timespan(1, 12, 31, 23, 59, 59);
		ASSERT_EQ(Test1 == Test1, true);
		ASSERT_EQ(Test1 <= Test2, true);
		ASSERT_EQ(Test3 >= Test2, true);

		NxEn::Timespan Test4 = Test1 + Test3;
		ASSERT_EQ(Test4, NxEn::Timespan(2, 13, 32, 24, 60, 60));
		Test4.Normalize();
		ASSERT_EQ(Test4, NxEn::Timespan(3, 2, 2, 1, 1, 0));
		
		NxEn::Timespan Test5 = Test4 - Test3;
		Test5.Normalize();
		ASSERT_EQ(Test5, Test1);
	}

	TEST(Type_Time, Timestamp)
	{
		NxEn::Timestamp Now = NxEn::Time::Now();
		ASSERT_EQ(Now.IsValid(), true);
		ASSERT_EQ(Now.IsFull(), true);

		NxEn::Timestamp Date = NxEn::Timestamp(2024, 11, 17, 23, 55, 30, 7, -1, true);
		ASSERT_EQ(Date.GetWeekDay(), "Sunday");
		ASSERT_EQ(Date.GetMonth(), "November");
		ASSERT_EQ(Date.ToString(), "Sunday - November 17 2024 - 23:55:30");

		NxEn::Timestamp Test1 = NxEn::Timestamp(1, 1, 1, 1, 1, 1);
		NxEn::Timestamp Test2 = NxEn::Timestamp(1, 2, 2, 2, 2, 2);
		NxEn::Timestamp Test3 = NxEn::Timestamp(1, 2, 3, 3, 3, 3);
		ASSERT_EQ(Test1.IsValid(), true);
		ASSERT_EQ(Test1.IsFull(), false);
		ASSERT_EQ(Test1 == Test1, true);
		ASSERT_EQ(Test1 <= Test2, true);
		ASSERT_EQ(Test3 >= Test2, true);

		NxEn::Timestamp Test4 = Test1 + NxEn::Timespan(1, 12, 31, 23, 59, 59);
		ASSERT_EQ(Test4, NxEn::Timestamp(3, 2, 2, 1, 1, 0));
		NxEn::Timestamp Test5 = Test4 - NxEn::Timespan(1, 12, 31, 23, 59, 59);
		ASSERT_EQ(Test5, Test1);

		NxEn::Timestamp Test6 = NxEn::Timestamp(2024, 11, 20, 23, 55, 00) + NxEn::Timespan(0, 1, 15, 0, 0, 0);
		ASSERT_EQ(Test6, NxEn::Timestamp(2025, 1, 5, 23, 55, 00));
		NxEn::Timestamp Test7 = NxEn::Timestamp(2025, 1, 5, 23, 55, 00) - NxEn::Timespan(0, 1, 15, 0, 0, 0);
		ASSERT_EQ(Test7, NxEn::Timestamp(2024, 11, 21, 23, 55, 00));
	}
}
