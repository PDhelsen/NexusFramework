#pragma once

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

#include <new>

#include <initializer_list>

#define NEXUS_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);

namespace NxFr
{
	template<typename T>
	using InitializerList = std::initializer_list<T>;
}
