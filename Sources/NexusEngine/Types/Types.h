#pragma once

#include "Platform/PlatformType.h"

typedef	PlaftormTypes::int8		int8;
typedef PlaftormTypes::int16	int16;
typedef PlaftormTypes::int32	int32;
typedef PlaftormTypes::int64	int64;

typedef PlaftormTypes::uint8	uint8;
typedef PlaftormTypes::uint16	uint16;
typedef PlaftormTypes::uint32	uint32;
typedef PlaftormTypes::uint64	uint64;

// TEMP: Remove once we have custom dictionary
#include "External/StandardLibrary.h"
template<typename T, typename U>
using Dictionary = std::unordered_map<T, U>;