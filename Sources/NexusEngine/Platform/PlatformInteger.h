#pragma once

#include "Platform/None/PlaftormNoneInteger.h"
#include "Platform/Windows/PlaftormWindowsInteger.h"

struct PlaftormInteger
{
#if NEXUS_WINDOWS
	typedef	PlatformWindowInteger::int8			int8;
	typedef PlatformWindowInteger::int16		int16;
	typedef PlatformWindowInteger::int32		int32;
	typedef PlatformWindowInteger::int64		int64;

	typedef PlatformWindowInteger::uint8		uint8;
	typedef PlatformWindowInteger::uint16		uint16;
	typedef PlatformWindowInteger::uint32		uint32;
	typedef PlatformWindowInteger::uint64		uint64;
#else
	typedef	PlatformNoneInteger::int8			int8;
	typedef PlatformNoneInteger::int16			int16;
	typedef PlatformNoneInteger::int32			int32;
	typedef PlatformNoneInteger::int64			int64;

	typedef PlatformNoneInteger::uint8			uint8;
	typedef PlatformNoneInteger::uint16			uint16;
	typedef PlatformNoneInteger::uint32			uint32;
	typedef PlatformNoneInteger::uint64			uint64;
#endif
};
