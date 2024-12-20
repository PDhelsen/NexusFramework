#pragma once

#if NEXUS_MSVC
	#include <intrin.h>
	
	#define NEXUS_FORCE_INLINE __forceinline
	#define NEXUS_DEBUGBREAK __debugbreak();

	#define NEXUS_FILE_NAME __FILE__
	#define NEXUS_LINE_NUMBER __LINE__
	#define NEXUS_FUNCTION_NAME __FUNCTION__
	#define NEXUS_FUNCTION_SIGNATURE __FUNCSIG__

	#define NEXUS_BITSCANFORWARD(Mask, Index, Result)	unsigned long Index;\
														uint64 Result = _BitScanForward(&Index, (unsigned long)Mask);
	#define NEXUS_BITSCANBACKWARD(Mask, Index, Result)	unsigned long Index;\
														uint64 Result = _BitScanReverse(&Index, (unsigned long)Mask);
#else
	#error "Compiler is not suppored"
#endif
