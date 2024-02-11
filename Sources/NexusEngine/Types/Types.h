#pragma once

// TEMP: Remove once we have custom dictionary
#include "External/StandardLibrary.h"

typedef	signed char			int8;
typedef signed short		int16;
typedef signed int	 		int32;
typedef signed long long	int64;

typedef unsigned char 		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

// TEMP: Remove once we have custom dictionary
template<typename T, typename U>
using Dictionary = std::unordered_map<T, U>;