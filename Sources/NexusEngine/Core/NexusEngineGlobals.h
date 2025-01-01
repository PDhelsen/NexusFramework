#pragma once

#include "Core/NexusEngineCore.h"
#include "Memory/Handle/HandleManager.h"
#include "Debug/Logger/Logger.h"
#include "Debug/Profiler/Stats/Stats.h"
#include "Debug/Profiler/Instruments/Instumentor.h"

namespace NxEn
{
	namespace Globals
	{
		NEXUS_ENGINE_API extern HandleManager* Handles;
		NEXUS_ENGINE_API extern Logger* Logs;
		NEXUS_ENGINE_API extern Stats* Statistiques;
		NEXUS_ENGINE_API extern Instruments* Instrumentor;
	}
}

