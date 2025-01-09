#include "Core/NexusTestPch.h"

namespace NxTs
{
	TEST(Stopwatch, Stopwatch)
	{
		NxFr::Stopwatch Stopwatch(true);

		NxFr::Platform::GetInstance()->Sleep(5);

		double Peek = Stopwatch.Peek(NxFr::Time::SecondToMilli);
		ASSERT_EQ(Peek >= 5, true);

		NxFr::Platform::GetInstance()->Sleep(5);

		double ElapsedTime = Stopwatch.Stop(NxFr::Time::SecondToMilli);
		ASSERT_EQ(ElapsedTime >= 5 && ElapsedTime >= Peek, true);
	}
}
