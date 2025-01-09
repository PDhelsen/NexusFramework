#pragma once

#include "Core/NexusFrameworkCore.h"
#include "IO/Path.h"

namespace NxEn
{
	namespace Paths
	{
		NEXUS_FRAMEWORK_API extern Path Root;

		NEXUS_FRAMEWORK_API extern Path Assets;
		NEXUS_FRAMEWORK_API extern Path Configs;
		NEXUS_FRAMEWORK_API extern Path Libraries;
		NEXUS_FRAMEWORK_API extern Path Resources;
		NEXUS_FRAMEWORK_API extern Path Scripts;
		NEXUS_FRAMEWORK_API extern Path Sources;

		NEXUS_FRAMEWORK_API extern Path Builds;
		NEXUS_FRAMEWORK_API extern Path Cooked;
		NEXUS_FRAMEWORK_API extern Path Saved;
	}
}
