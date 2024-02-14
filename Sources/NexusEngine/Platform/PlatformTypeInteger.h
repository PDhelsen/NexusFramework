#pragma once

// Define the bytes precision of the integer types.
// This file forward the platform-dependant definition

#include "Platform/Windows/PlaftormTypeIntegerWindows.h"

struct PlaftormTypeInteger
{
#if NEXUS_WINDOWS
	typedef	PlatformTypeIntegerWindows::int8		int8;
	typedef PlatformTypeIntegerWindows::int16		int16;
	typedef PlatformTypeIntegerWindows::int32		int32;
	typedef PlatformTypeIntegerWindows::int64		int64;

	typedef PlatformTypeIntegerWindows::uint8		uint8;
	typedef PlatformTypeIntegerWindows::uint16		uint16;
	typedef PlatformTypeIntegerWindows::uint32		uint32;
	typedef PlatformTypeIntegerWindows::uint64		uint64;
#else
	typedef	signed char			int8;
	typedef signed short		int16;
	typedef signed int	 		int32;
	typedef signed long long	int64;

	typedef unsigned char 		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long	uint64;
#endif
};