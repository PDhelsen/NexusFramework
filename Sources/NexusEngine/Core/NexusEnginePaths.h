#pragma once

#include "Core/NexusEngineCore.h"
#include "IO/Path.h"

namespace NxEn
{
	namespace Paths
	{
		NEXUS_ENGINE_API extern Path Root;

		NEXUS_ENGINE_API extern Path Assets;
		NEXUS_ENGINE_API extern Path Configs;
		NEXUS_ENGINE_API extern Path Libraries;
		NEXUS_ENGINE_API extern Path Resources;
		NEXUS_ENGINE_API extern Path Scripts;
		NEXUS_ENGINE_API extern Path Sources;

		NEXUS_ENGINE_API extern Path Builds;
		NEXUS_ENGINE_API extern Path Cooked;
		NEXUS_ENGINE_API extern Path Saved;
	}
}
