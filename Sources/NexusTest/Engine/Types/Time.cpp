#include "Core/NexusTestPch.h"

#include "Types/Timestamp.h"
#include "Application/Time.h"

namespace NxTs
{
	TEST(Type_Time, Timestamp)
	{
		NxEn::Timestamp Now = NxEn::Time::GetInstance()->Now();
		ASSERT_EQ(Now.IsValid(), true);
		ASSERT_EQ(Now.IsFull(), true);

		NxEn::Timestamp Test1 = NxEn::Timestamp(1, 1, 1, 1, 1, 1);
		NxEn::Timestamp Test2 = NxEn::Timestamp(1, 2, 2, 2, 2, 2);
		NxEn::Timestamp Test3 = NxEn::Timestamp(1, 2, 3, 3, 3, 3);
		ASSERT_EQ(Test1.IsValid(), true);
		ASSERT_EQ(Test1.IsFull(), false);
		ASSERT_EQ(Test1 == Test1, true);
		ASSERT_EQ(Test1 <= Test2, true);
		ASSERT_EQ(Test3 >= Test2, true);

		NxEn::Timestamp Date = NxEn::Timestamp(2024, 11, 17, 23, 55, 30, 7, -1, true);
		ASSERT_EQ(Date.GetWeekDay(), "Sunday");
		ASSERT_EQ(Date.GetMonth(), "November");
		ASSERT_EQ(Date.ToString(), "Sunday - November 17 2024 - 23:55:30");
	}
}
