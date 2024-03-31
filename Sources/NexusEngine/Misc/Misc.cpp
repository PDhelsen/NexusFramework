#include "Core/NexusEnginePch.h"
#include "Misc.h"

namespace NxEn
{
	bool IsPowerOfTwo(uint64 Number)
	{
		return (Number & (Number - 1)) == 0;
	}
	
	int32 LogTwoPowerOfTwo(uint32 Number)
	{
		NEXUS_ASSERT(IsPowerOfTwo(Number), "Number is not a power of 2");

		NEXUS_BITSCANFORWARD(Number, Index, Result)
		return Result > 0 ? Index : -1;
	}
}