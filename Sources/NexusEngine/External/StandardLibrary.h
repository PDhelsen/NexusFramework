#pragma once

#include <iostream>

#include <stdarg.h>
#define NEXUS_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);

// TEMP: Remove once we have custom dictionary
#include <unordered_map>