#pragma once

#include "NexusFramework/Platform/None/PlaftormNoneInteger.h"
#include "NexusFramework/Platform/Windows/PlaftormWindowsInteger.h"

struct PlaftormInteger
{
#if NEXUS_WINDOWS
	using int8		= PlatformWindowInteger::int8;
	using int16		= PlatformWindowInteger::int16;
	using int32		= PlatformWindowInteger::int32;
	using int64		= PlatformWindowInteger::int64;

	using uint8		= PlatformWindowInteger::uint8;
	using uint16	= PlatformWindowInteger::uint16;
	using uint32	= PlatformWindowInteger::uint32;
	using uint64	= PlatformWindowInteger::uint64;
#else
	using int8		= PlatformNoneInteger::int8;
	using int16		= PlatformNoneInteger::int16;
	using int32		= PlatformNoneInteger::int32;
	using int64		= PlatformNoneInteger::int64;

	using uint8		= PlatformNoneInteger::uint8;
	using uint16	= PlatformNoneInteger::uint16;
	using uint32	= PlatformNoneInteger::uint32;
	using uint64	= PlatformNoneInteger::uint64;
#endif
};
