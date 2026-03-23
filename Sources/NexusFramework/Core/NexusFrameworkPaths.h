#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/IO/Path.h"

namespace NxFr
{
	namespace Paths
	{
		NEXUS_FRAMEWORK_API extern String Root;

		NEXUS_FRAMEWORK_API extern String Assets;
		NEXUS_FRAMEWORK_API extern String Configs;
		NEXUS_FRAMEWORK_API extern String Libraries;
		NEXUS_FRAMEWORK_API extern String Resources;
		NEXUS_FRAMEWORK_API extern String Scripts;
		NEXUS_FRAMEWORK_API extern String Sources;

		NEXUS_FRAMEWORK_API extern String Builds;
		NEXUS_FRAMEWORK_API extern String Cooked;
		NEXUS_FRAMEWORK_API extern String Saved;

		NEXUS_FRAMEWORK_API extern void SetupPathsAndFolders();
		NEXUS_FRAMEWORK_API extern void CleanupFolders();
	}
}
