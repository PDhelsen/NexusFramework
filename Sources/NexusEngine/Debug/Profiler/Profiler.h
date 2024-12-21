#pragma once

#include "Debug/Profiler/Instruments/Instumentor.h"
#include "Debug/Profiler/Stats/Stats.h"

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_PROFILE_SCOPE(Name) NEXUS_INSTUMENT_SCOPE(Name);
#define NEXUS_PROFILE_FUNCTION() NEXUS_INSTUMENT_FUNCTION();
#elif NEXUS_DISTRIB
#define NEXUS_PROFILE_SCOPE(Name)
#define NEXUS_PROFILE_FUNCTION()
#endif
