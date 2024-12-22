#pragma once

#include "Core/NexusEngine.h"
#include "Platform/PlatformInteger.h"

using int8		= PlaftormInteger::int8;
using int16		= PlaftormInteger::int16;
using int32		= PlaftormInteger::int32;
using int64		= PlaftormInteger::int64;

using uint8		= PlaftormInteger::uint8;
using uint16	= PlaftormInteger::uint16;
using uint32	= PlaftormInteger::uint32;
using uint64	= PlaftormInteger::uint64;

namespace NxEn
{
	using GUID = uint64;
	using Byte = uint8;
}

#define NEXUS_BIT_CHECK_SET(Type)\
inline bool CheckBit(Type Value, Type Offset) { return Value & (Type(1) << Offset); }\
inline Type SetBit1(Type Value, Type Offset) { return Value |= Type(1) << Offset; }\
inline Type SetBit0(Type Value, Type Offset) { return Value &= ~(Type(1) << Offset); }\
inline Type SetBit(Type Value, Type Offset, bool State) { return State ? SetBit1(Value, Offset) : SetBit0(Value, Offset); }\

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

	NEXUS_ENGINE_API constexpr	int8	MinI8();
	NEXUS_ENGINE_API constexpr	int16	MinI16();
	NEXUS_ENGINE_API constexpr	int32	MinI32();
	NEXUS_ENGINE_API constexpr	int64	MinI64();
	NEXUS_ENGINE_API constexpr	uint8	MinUI8();
	NEXUS_ENGINE_API constexpr	uint16	MinUI16();
	NEXUS_ENGINE_API constexpr	uint32	MinUI32();
	NEXUS_ENGINE_API constexpr	uint64	MinUI64();
	NEXUS_ENGINE_API constexpr	int8	MaxI8();
	NEXUS_ENGINE_API constexpr	int16	MaxI16();
	NEXUS_ENGINE_API constexpr	int32	MaxI32();
	NEXUS_ENGINE_API constexpr	int64	MaxI64();
	NEXUS_ENGINE_API constexpr	uint8	MaxUI8();
	NEXUS_ENGINE_API constexpr	uint16	MaxUI16();
	NEXUS_ENGINE_API constexpr	uint32	MaxUI32();
	NEXUS_ENGINE_API constexpr	uint64	MaxUI64();
}
