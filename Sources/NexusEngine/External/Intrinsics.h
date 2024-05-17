#pragma once

// All the compiler intrinsics functions

#if NEXUS_MSVC
	#include <intrin.h>
	
	#define NEXUS_FORCE_INLINE __forceinline

	#define NEXUS_BITSCANFORWARD(Mask, Index, Result)	unsigned long Index;\
														uint64 Result = _BitScanForward(&Index, (unsigned long)Mask);
	#define NEXUS_BITSCANBACKWARD(Mask, Index, Result)	unsigned long Index;\
														uint64 Result = _BitScanReverse(&Index, (unsigned long)Mask);
#else
	#error "Compiler is not suppored"
#endif