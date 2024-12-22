#pragma once

#include "External/Intrinsics.h"
#include "Debug/Logger.h"

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_BREAK NEXUS_DEBUGBREAK
#elif NEXUS_DISTRIB
	#define NEXUS_BREAK
#endif

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_ASSERT_INSTANCE(Instance, Condition, Msg, ...) if (!(Condition)) { NEXUS_LOG_INSTANCE(Instance, Fatal, NxEn::LoggerChannel::Assert, Msg, __VA_ARGS__); NEXUS_BREAK; }

	#define NEXUS_ASSERT(Condition, Msg, ...) if (!(Condition)) { NEXUS_LOG(Fatal, NxEn::LoggerChannel::Assert, Msg, __VA_ARGS__); NEXUS_BREAK; }
#elif NEXUS_DISTRIB
	#define NEXUS_ASSERT_INSTANCE(Instance, Condition, Msg, ...)

	#define NEXUS_ASSERT(Condition, Msg, ...)
#endif

#define NEXUS_ASSERT_STATIC(condition, Msg) static_assert(condition, Msg)
