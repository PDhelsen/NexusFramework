#include "Core/NexusTestPch.h"

namespace NxTs
{
	TEST(Stopwatch, Stopwatch)
	{
		NxEn::Stopwatch Stopwatch(true);

		NxEn::Platform::GetInstance()->Sleep(5);

		double Peek = Stopwatch.Peek(NxEn::Time::SecondToMilli);
		ASSERT_EQ(Peek >= 5, true);

		NxEn::Platform::GetInstance()->Sleep(5);

		double ElapsedTime = Stopwatch.Stop(NxEn::Time::SecondToMilli);
		ASSERT_EQ(ElapsedTime >= 5 && ElapsedTime >= Peek, true);
	}
}
