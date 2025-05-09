#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Platform/PlatformInteger.h"
#include "NexusFramework/External/StandardLibrary.h"

using int8		= PlaftormInteger::int8;
using int16		= PlaftormInteger::int16;
using int32		= PlaftormInteger::int32;
using int64		= PlaftormInteger::int64;

using uint8		= PlaftormInteger::uint8;
using uint16	= PlaftormInteger::uint16;
using uint32	= PlaftormInteger::uint32;
using uint64	= PlaftormInteger::uint64;

namespace NxFr
{
	using GUID = uint64;
	using Byte = uint8;
}

#define NEXUS_BIT_CHECK_SET(Type)\
inline bool CheckBit(Type Value, Type Offset) { return Value & (Type(1) << Offset); }\
inline Type SetBit1(Type Value, Type Offset) { return Value |= Type(1) << Offset; }\
inline Type SetBit0(Type Value, Type Offset) { return Value &= ~(Type(1) << Offset); }\
inline Type SetBit(Type Value, Type Offset, bool State) { return State ? SetBit1(Value, Offset) : SetBit0(Value, Offset); }\

namespace NxFr::Integer
{
	NEXUS_BIT_CHECK_SET(int8);
	NEXUS_BIT_CHECK_SET(int16);
	NEXUS_BIT_CHECK_SET(int32);
	NEXUS_BIT_CHECK_SET(int64);
	NEXUS_BIT_CHECK_SET(uint8);
	NEXUS_BIT_CHECK_SET(uint16);
	NEXUS_BIT_CHECK_SET(uint32);
	NEXUS_BIT_CHECK_SET(uint64);

	constexpr int8 MinI8 = CHAR_MIN;
	constexpr int16 MinI16 = SHRT_MIN;
	constexpr int32 MinI32 = INT_MIN;
	constexpr int64 MinI64 = LLONG_MIN;
	constexpr uint8 MinUI8 = 0;
	constexpr uint16 MinUI16 = 0;
	constexpr uint32 MinUI32 = 0;
	constexpr uint64 MinUI64 = 0;
	constexpr int8 MaxI8 = CHAR_MAX;
	constexpr int16 MaxI16 = SHRT_MAX;
	constexpr int32 MaxI32 = INT_MAX;
	constexpr int64 MaxI64 = LLONG_MAX;
	constexpr uint8 MaxUI8 = UCHAR_MAX;
	constexpr uint16 MaxUI16 = USHRT_MAX;
	constexpr uint32 MaxUI32 = UINT_MAX;
	constexpr uint64 MaxUI64 = ULLONG_MAX;
}
