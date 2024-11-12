#pragma once

#include "Platform/PlatformInteger.h"

using int8		= PlaftormInteger::int8;
using int16		= PlaftormInteger::int16;
using int32		= PlaftormInteger::int32;
using int64		= PlaftormInteger::int64;

using uint8		= PlaftormInteger::uint8;
using uint16	= PlaftormInteger::uint16;
using uint32	= PlaftormInteger::uint32;
using uint64	= PlaftormInteger::uint64;

#define NEXUS_BIT_CHECK_SET(Type)\
inline bool CheckBit(Type Value, Type Offset) { return Value & (Type(1) << Offset); }\
inline Type SetBit1(Type Value, Type Offset) { return Value |= Type(1) << Offset; }\
inline Type SetBit0(Type Value, Type Offset) { return Value &= ~(Type(1) << Offset); }\
inline Type SetBit(Type Value, Type Offset, bool State) { return State ? SetBit1(Value, Offset) : SetBit0(Value, Offset); }\

namespace NxEn
{
	using GUID = uint64;
	using Byte = uint8;
}

namespace NxEn::Integer
{
	NEXUS_BIT_CHECK_SET(int8);
	NEXUS_BIT_CHECK_SET(int16);
	NEXUS_BIT_CHECK_SET(int32);
	NEXUS_BIT_CHECK_SET(int64);
	NEXUS_BIT_CHECK_SET(uint8);
	NEXUS_BIT_CHECK_SET(uint16);
	NEXUS_BIT_CHECK_SET(uint32);
	NEXUS_BIT_CHECK_SET(uint64);
}
