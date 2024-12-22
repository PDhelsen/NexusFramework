#include "Core/NexusTestPch.h"

#include "Debug/Stopwatch.h"
#include "Application/Time.h"
#include "Platform/Platform.h"

namespace NxTs
{
	TEST(Stopwatch, Stopwatch)
	{
		NxEn::Stopwatch Stopwatch(true);

		NxEn::Platform::GetInstance()->Sleep(5);

		double ElapsedTime = Stopwatch.Stop(NxEn::Time::SecondToMilli);
		ASSERT_EQ(ElapsedTime >= 5, true);
	}
}
