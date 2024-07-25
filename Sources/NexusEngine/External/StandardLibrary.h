#pragma once

//TODO: Evaluate - Project - C lib vs C++ lib
#include <iostream>
#include <ctime>

#include <stdarg.h>
#define NEXUS_VA(Format, Function)	va_list ArgList;\
									va_start(ArgList, Format);\
									Function;\
									va_end(ArgList);
