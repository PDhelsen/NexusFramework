#include "Core/NexusTestPch.h"

#include "Debug/Stopwatch.h"
#include <windows.h>

namespace NxTs
{
	TEST(Stopwatch, Stopwatch)
	{
		NxEn::Stopwatch Stopwatch(true);

		uint64 Count = 0;
		for (uint64 Iter = 0; Iter < 10000; ++Iter)
		{
			Count++;
		}

		double ElapsedTime = Stopwatch.Stop(1000);
		ASSERT_EQ(ElapsedTime > 0.0, true);
	}
}
