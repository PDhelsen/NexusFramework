#pragma once

// C Include
#include <cassert>
#include <cctype>
#include <cinttypes>
#include <cfloat>
#include <cstring>
#include <cstddef>
#include <cstdalign>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <cmath>
#include <ctime>
// C++ Include
#include <new>
#include <initializer_list>

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

#define NX_INTERGER_MIN_I8 CHAR_MIN;
#define NX_INTERGER_MIN_I16 SHRT_MIN;
#define NX_INTERGER_MIN_I32 INT_MIN;
#define NX_INTERGER_MIN_I64 LLONG_MIN;
#define NX_INTERGER_MIN_UI8 0;
#define NX_INTERGER_MIN_UI16 0;
#define NX_INTERGER_MIN_UI32 0;
#define NX_INTERGER_MIN_UI64 0;
#define NX_INTERGER_MAX_I8 CHAR_MAX;
#define NX_INTERGER_MAX_I16 SHRT_MAX;
#define NX_INTERGER_MAX_I32 INT_MAX;
#define NX_INTERGER_MAX_I64 LLONG_MAX;
#define NX_INTERGER_MAX_UI8 UCHAR_MAX;
#define NX_INTERGER_MAX_UI16 USHRT_MAX;
#define NX_INTERGER_MAX_UI32 UINT_MAX;
#define NX_INTERGER_MAX_UI64 ULLONG_MAX;
#define NX_DECIMAL_MIN_FLOAT FLT_MIN
#define NX_DECIMAL_MIN_DOUBLE DBL_MIN
#define NX_DECIMAL_MAX_FLOAT FLT_MAX
#define NX_DECIMAL_MAX_DOUBLE DBL_MAX
#define NX_DECIMAL_INF_FLOAT INFINITY;
#define NX_DECIMAL_INF_DOUBLE INFINITY;
#define NX_DECIMAL_EPS_FLOAT 1e-4f
#define NX_DECIMAL_EPS_DOUBLE 1e-9f

#define NX_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);

namespace NxFr
{
	template<typename T>
	using InitializerList = std::initializer_list<T>;
}
