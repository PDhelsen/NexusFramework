#pragma once

#include <iostream>

#include <stdarg.h>
#define NEXUS_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);

// TODO: Replace with custom implementation
#include <unordered_map>
template<typename T, typename U>
using Dictionary = std::unordered_map<T, U>;