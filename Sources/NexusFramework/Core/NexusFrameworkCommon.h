#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	class Logger;
	class Stats;
	class Instruments;
	class MemoryTracker;
	class Arguments;
	class Platform;

	namespace Globals
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
			NEXUS_FRAMEWORK_API extern String Temp;
		}

		namespace Debug
		{
			NEXUS_FRAMEWORK_API extern Logger* Logs;
			NEXUS_FRAMEWORK_API extern Stats* Statistiques;
			NEXUS_FRAMEWORK_API extern Instruments* Instrumentor;
			NEXUS_FRAMEWORK_API extern MemoryTracker* Memory;
		}

		NEXUS_FRAMEWORK_API extern Arguments* Args;
		NEXUS_FRAMEWORK_API extern Platform* PlatformTarget;
		NEXUS_FRAMEWORK_API extern uint64 MainThreadId;

		NEXUS_FRAMEWORK_API void Initialize(uint64 ArgC, char* ArgV[]);
		NEXUS_FRAMEWORK_API void Shutdown();

		NEXUS_FRAMEWORK_API void CreatePlatform();
		NEXUS_FRAMEWORK_API void CreateArgs(uint64 ArgC, char* ArgV[]);
		NEXUS_FRAMEWORK_API void DestroyArgs();
		NEXUS_FRAMEWORK_API void CreatePathsAndFolders();
		NEXUS_FRAMEWORK_API void DestroyTempFolder();
		NEXUS_FRAMEWORK_API void CreateDebug(StringView Path);
		NEXUS_FRAMEWORK_API void DestroyDebug();
	}
}
