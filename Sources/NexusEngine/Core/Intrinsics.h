#pragma once

#if NEXUS_MSVC
	#define NEXUS_BITSCANFORWARD(Mask, Index, Result)	unsigned long Index;\
														uint64 Result = _BitScanForward(&Index, (unsigned long)Mask);
	#define NEXUS_BITSCANBACKWARD(Mask, Index, Result)	unsigned long Index;\
														uint64 Result = _BitScanReverse(&Index, (unsigned long)Mask);
#else
	#error "Compiler is not suppored"
#endif