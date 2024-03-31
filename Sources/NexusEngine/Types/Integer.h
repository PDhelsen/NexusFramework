#pragma once

// Define the bytes precision of the integer types.
// This file forward the current platform definition

#include "Platform/PlatformTypeInteger.h"

typedef	PlaftormTypeInteger::int8		int8;
typedef PlaftormTypeInteger::int16		int16;
typedef PlaftormTypeInteger::int32		int32;
typedef PlaftormTypeInteger::int64		int64;

typedef PlaftormTypeInteger::uint8		uint8;
typedef PlaftormTypeInteger::uint16		uint16;
typedef PlaftormTypeInteger::uint32		uint32;
typedef PlaftormTypeInteger::uint64		uint64;

#define NEXUS_BIT_CHECK_SET(Type)\
inline bool CheckBit(Type Value, Type Offset) { return Value & (Type(1) << Offset); }\
inline Type SetBit1(Type Value, Type Offset) { return Value |= Type(1) << Offset; }\
inline Type SetBit0(Type Value, Type Offset) { return Value &= ~(Type(1) << Offset); }\
inline Type SetBit(Type Value, Type Offset, bool State) { return State ? SetBit1(Value, Offset) : SetBit0(Value, Offset); }\

NEXUS_BIT_CHECK_SET(int8);
NEXUS_BIT_CHECK_SET(int16);
NEXUS_BIT_CHECK_SET(int32);
NEXUS_BIT_CHECK_SET(int64);
NEXUS_BIT_CHECK_SET(uint8);
NEXUS_BIT_CHECK_SET(uint16);
NEXUS_BIT_CHECK_SET(uint32);
NEXUS_BIT_CHECK_SET(uint64);
