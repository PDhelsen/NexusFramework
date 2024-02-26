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

#define BIT_CHECK_SET(Type)\
inline bool CheckBit(Type Value, Type Offset) { return Value & (Type(1) << Offset); }\
inline void SetBit(Type& Value, Type Offset, bool State) \
{\
	Type Mask = Type(1) << Offset;\
	if (State)\
	{\
		Value |= Mask;\
	}\
	else\
	{\
		Value &= ~(Mask);\
	}\
}\

BIT_CHECK_SET(int8);
BIT_CHECK_SET(int16);
BIT_CHECK_SET(int32);
BIT_CHECK_SET(int64);
BIT_CHECK_SET(uint8);
BIT_CHECK_SET(uint16);
BIT_CHECK_SET(uint32);
BIT_CHECK_SET(uint64);