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

#include <cmath>
#include <ctime>

#include <new>
#include <iostream>

#define NEXUS_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);
