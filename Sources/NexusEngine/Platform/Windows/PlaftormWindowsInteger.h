#pragma once

// Define the bytes precision of the integer types.
// This file define the Win64 integer type

struct PlatformWindowInteger
{
	typedef	signed char			int8;
	typedef signed short		int16;
	typedef signed int	 		int32;
	typedef signed long long	int64;

	typedef unsigned char 		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long	uint64;
};