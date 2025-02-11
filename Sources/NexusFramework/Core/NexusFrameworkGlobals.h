#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"
#include "NexusFramework/Debug/Logger/Logger.h"
#include "NexusFramework/Debug/Profiler/Stats/Stats.h"
#include "NexusFramework/Debug/Profiler/Instruments/Instuments.h"

namespace NxFr
{
	namespace Globals
	{
		NEXUS_FRAMEWORK_API extern Logger* Logs;
		NEXUS_FRAMEWORK_API extern Stats* Statistiques;
		NEXUS_FRAMEWORK_API extern Instruments* Instrumentor;
	}
}

