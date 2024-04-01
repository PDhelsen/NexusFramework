#pragma once

// All the needed standard library features should be included here

#include <iostream>
#include <ctime>

#include <stdarg.h>
#define NEXUS_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);

// TEMP: Remove - Container - Once we have custom dictionary
#include <unordered_map>
template<typename T, typename U>
using Dictionary = std::unordered_map<T, U>;