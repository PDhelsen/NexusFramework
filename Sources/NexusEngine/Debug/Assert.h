#pragma once

#include "External/Intrinsics.h"
#include "Debug/Logger.h"

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_BREAK NEXUS_DEBUGBREAK
#elif NEXUS_DISTRIB
	#define NEXUS_BREAK
#endif

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_ASSERT(condition, Msg, ...) if (!(condition)) { NEXUS_LOG(Engine, Fatal, NxEn::LoggerChannel::Assert, Msg, __VA_ARGS__); NEXUS_BREAK; }
#elif NEXUS_DISTRIB
	#define NEXUS_ASSERT(condition, Msg, ...)
#endif

#define NEXUS_ASSERT_STATIC(condition, Msg) static_assert(condition, Msg)
