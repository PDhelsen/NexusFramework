#pragma once

#include "Platform/Windows/PlaftormTypesWindows.h"

struct PlaftormTypes
{
#if NEXUS_WINDOWS
	typedef	PlaftormTypesWindows::int8		int8;
	typedef PlaftormTypesWindows::int16		int16;
	typedef PlaftormTypesWindows::int32		int32;
	typedef PlaftormTypesWindows::int64		int64;

	typedef PlaftormTypesWindows::uint8		uint8;
	typedef PlaftormTypesWindows::uint16	uint16;
	typedef PlaftormTypesWindows::uint32	uint32;
	typedef PlaftormTypesWindows::uint64	uint64;
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