#pragma once

// Define the bytes precision of the integer types.
// This file forward the platform-dependant definition

#include "Platform/None/PlaftormTypeIntegerNone.h"
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
	typedef	PlatformTypeIntegerNone::int8			int8;
	typedef PlatformTypeIntegerNone::int16			int16;
	typedef PlatformTypeIntegerNone::int32			int32;
	typedef PlatformTypeIntegerNone::int64			int64;

	typedef PlatformTypeIntegerNone::uint8			uint8;
	typedef PlatformTypeIntegerNone::uint16			uint16;
	typedef PlatformTypeIntegerNone::uint32			uint32;
	typedef PlatformTypeIntegerNone::uint64			uint64;
#endif
};