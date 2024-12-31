#pragma once

#include "External/Intrinsics.h"
#include "Debug/Logger/Logger.h"

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_BREAK NEXUS_DEBUGBREAK
#elif NEXUS_DISTRIB
	#define NEXUS_BREAK
#endif

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_ASSERT_INSTANCE(Instance, Condition, Channel, Msg, ...) if (!(Condition)) { \
	NEXUS_LOG_INSTANCE(Instance, Fatal, Channel, Msg, __VA_ARGS__); \
	NEXUS_LOG_INSTANCE(Instance, Fatal, Channel, "%s %s %d", NEXUS_FUNCTION_SIGNATURE, NEXUS_FILE_NAME, NEXUS_LINE_NUMBER); \
	NEXUS_BREAK; }

	#define NEXUS_ASSERT(Condition, Channel, Msg, ...) if (!(Condition)) { \
	NEXUS_LOG(Fatal, Channel, Msg, __VA_ARGS__); \
	NEXUS_LOG(Fatal, Channel, "%s %s %d", NEXUS_FUNCTION_SIGNATURE, NEXUS_FILE_NAME, NEXUS_LINE_NUMBER); \
	NEXUS_BREAK; }
#elif NEXUS_DISTRIB
	#define NEXUS_ASSERT_INSTANCE(Instance, Condition, Msg, ...)

	#define NEXUS_ASSERT(Condition, Msg, ...)
#endif

#define NEXUS_ASSERT_STATIC(condition, Msg) static_assert(condition, Msg)
