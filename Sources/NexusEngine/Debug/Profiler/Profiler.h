#pragma once

#include "Debug/Profiler/Instruments/Instuments.h"
#include "Debug/Profiler/Stats/Stats.h"

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_PROFILE_SCOPE_INSTANCE(Instance, Name) NEXUS_INSTUMENT_SCOPE_INSTANCE(Instance, Name);
	#define NEXUS_PROFILE_FUNCTION_INSTANCE(Instance) NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance);

	#define NEXUS_PROFILE_SCOPE(Name) NEXUS_INSTUMENT_SCOPE(Name);
	#define NEXUS_PROFILE_FUNCTION() NEXUS_INSTUMENT_FUNCTION();
#elif NEXUS_DISTRIB
	#define NEXUS_PROFILE_SCOPE_INSTANCE(Name, Instance)
	#define NEXUS_PROFILE_FUNCTION_INSTANCE(Instance)

	#define NEXUS_PROFILE_SCOPE(Name)
	#define NEXUS_PROFILE_FUNCTION()
#endif
