#pragma once

#if NX_MSVC
	#include <intrin.h>
	
	#define NX_FORCE_INLINE __forceinline
	#define NX_DEBUGBREAK __debugbreak()

	#define NX_FILE_NAME __FILE__
	#define NX_LINE_NUMBER __LINE__
	#define NX_FUNCTION_NAME __FUNCTION__
	#define NX_FUNCTION_SIGNATURE __FUNCSIG__

	#define NX_BITSCANFORWARD(Mask, Index, Result)	unsigned long Index;\
														unsigned char Result = _BitScanForward(&Index, (unsigned long)Mask);
	#define NX_BITSCANBACKWARD(Mask, Index, Result)	unsigned long Index;\
														unsigned char Result = _BitScanReverse(&Index, (unsigned long)Mask);

	#define NX_THREAD_INTERLOCKED_INCREMENT _InterlockedIncrement64
	#define NX_THREAD_INTERLOCKED_DECREMENT _InterlockedDecrement64
	#define NX_THREAD_INTERLOCKED_ADD _InterlockedExchangeAdd64
	#define NX_THREAD_INTERLOCKED_EXCHANGE _InterlockedExchange64
	#define NX_THREAD_INTERLOCKED_COMPAREEXCHANGE _InterlockedCompareExchange64

#else
	#error "Compiler is not suppored"
#endif
