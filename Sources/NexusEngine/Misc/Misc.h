#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	// Regroup general unrelated function that are not big enough to be their own things.

	NEXUS_ENGINE_API bool IsPowerOfTwo(uint64 Number);
	NEXUS_ENGINE_API int32 LogTwoPowerOfTwo(uint32 Number);
}