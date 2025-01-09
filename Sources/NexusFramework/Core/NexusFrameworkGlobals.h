#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Memory/Handle/HandleManager.h"
#include "Debug/Logger/Logger.h"
#include "Debug/Profiler/Stats/Stats.h"
#include "Debug/Profiler/Instruments/Instuments.h"

namespace NxEn
{
	namespace Globals
	{
		NEXUS_FRAMEWORK_API extern HandleManager* Handles;
		NEXUS_FRAMEWORK_API extern Logger* Logs;
		NEXUS_FRAMEWORK_API extern Stats* Statistiques;
		NEXUS_FRAMEWORK_API extern Instruments* Instrumentor;
	}
}

