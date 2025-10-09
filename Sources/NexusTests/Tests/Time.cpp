#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Time, Timespan)
	{
		NxFr::Timespan Span = NxFr::Timespan(1, 10, 5, 23, 55, 30);
		ASSERT_EQ(NxFr::StringUtility::ToString(Span), "0001y10m05d - 23h55m30s");

		NxFr::Timespan Test1 = NxFr::Timespan(1, 1, 1, 1, 1, 1);
		NxFr::Timespan Test2 = NxFr::Timespan(1, 2, 2, 2, 2, 2);
		NxFr::Timespan Test3 = NxFr::Timespan(1, 12, 31, 23, 59, 59);
		ASSERT_EQ(Test1 == Test1, true);
		ASSERT_EQ(Test1 <= Test2, true);
		ASSERT_EQ(Test3 >= Test2, true);

		NxFr::Timespan Test4 = Test1 + Test3;
		ASSERT_EQ(Test4, NxFr::Timespan(2, 13, 32, 24, 60, 60));
		Test4.Normalize();
		ASSERT_EQ(Test4, NxFr::Timespan(3, 2, 2, 1, 1, 0));

		NxFr::Timespan Test5 = Test4 - Test3;
		Test5.Normalize();
		ASSERT_EQ(Test5, Test1);
	}

	TEST(Time, Timestamp)
	{
		NxFr::Timestamp Now = NxFr::Time::Now();
		ASSERT_EQ(Now.IsValid(), true);
		ASSERT_EQ(Now.IsFull(), true);

		NxFr::Timestamp Date = NxFr::Timestamp(2024, 11, 17, 23, 55, 30, 7, -1, true);
		ASSERT_EQ(Date.GetWeekDay(), "Sunday");
		ASSERT_EQ(Date.GetMonth(), "November");
		ASSERT_EQ(NxFr::StringUtility::ToString(Date), "Sunday - November 17 2024 - 23:55:30");

		NxFr::Timestamp Test1 = NxFr::Timestamp(1, 1, 1, 1, 1, 1);
		NxFr::Timestamp Test2 = NxFr::Timestamp(1, 2, 2, 2, 2, 2);
		NxFr::Timestamp Test3 = NxFr::Timestamp(1, 2, 3, 3, 3, 3);
		ASSERT_EQ(Test1.IsValid(), true);
		ASSERT_EQ(Test1.IsFull(), false);
		ASSERT_EQ(Test1 == Test1, true);
		ASSERT_EQ(Test1 <= Test2, true);
		ASSERT_EQ(Test3 >= Test2, true);

		NxFr::Timestamp Test4 = Test1 + NxFr::Timespan(1, 12, 31, 23, 59, 59);
		ASSERT_EQ(Test4, NxFr::Timestamp(3, 2, 2, 1, 1, 0));
		NxFr::Timestamp Test5 = Test4 - NxFr::Timespan(1, 12, 31, 23, 59, 59);
		ASSERT_EQ(Test5, Test1);

		NxFr::Timestamp Test6 = NxFr::Timestamp(2024, 11, 20, 23, 55, 00) + NxFr::Timespan(0, 1, 15, 0, 0, 0);
		ASSERT_EQ(Test6, NxFr::Timestamp(2025, 1, 5, 23, 55, 00));
		NxFr::Timestamp Test7 = NxFr::Timestamp(2025, 1, 5, 23, 55, 00) - NxFr::Timespan(0, 1, 15, 0, 0, 0);
		ASSERT_EQ(Test7, NxFr::Timestamp(2024, 11, 21, 23, 55, 00));
	}
}
