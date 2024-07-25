#pragma once

#include "Debug/Logger.h"

#if NEXUS_DEBUG || NEXUS_RELEASE
	#if NEXUS_WINDOWS
		#define NEXUS_BREAK __debugbreak();
	#else
		#define NEXUS_BREAK
	#endif
#elif NEXUS_DISTRIB
	#define NEXUS_BREAK
#endif

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_ASSERT(condition, Msg, ...) if (!(condition)) { NEXUS_LOG(Engine, Fatal, NxEn::Logger::ChannelAssert, Msg, __VA_ARGS__); NEXUS_BREAK; }
#elif NEXUS_DISTRIB
	#define NEXUS_ASSERT(condition, Msg, ...)
#endif

#define NEXUS_ASSERT_STATIC(condition, Msg) static_assert(condition, Msg)
