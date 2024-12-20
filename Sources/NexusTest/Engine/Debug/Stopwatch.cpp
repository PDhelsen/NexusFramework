#include "Core/NexusTestPch.h"

#include "Debug/Stopwatch.h"

namespace NxTs
{
	TEST(Stopwatch, Stopwatch)
	{
		NxEn::Stopwatch Stopwatch(true);

		for (uint64 Iter = 0; Iter < 1000; ++Iter)
		{
			void* Dum = NxEn::Memory::Allocate(1024);
			NxEn::Memory::Free(Dum);
		}

		double ElapsedTime = Stopwatch.Stop(1000000);
		ASSERT_EQ(ElapsedTime > 0.0, true);
	}
}
