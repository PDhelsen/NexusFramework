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
			NX_FRAMEWORK_API extern String Root;
			NX_FRAMEWORK_API extern String Assets;
			NX_FRAMEWORK_API extern String Configs;
			NX_FRAMEWORK_API extern String Libraries;
			NX_FRAMEWORK_API extern String Resources;
			NX_FRAMEWORK_API extern String Scripts;
			NX_FRAMEWORK_API extern String Sources;
			NX_FRAMEWORK_API extern String Builds;
			NX_FRAMEWORK_API extern String Cooked;
			NX_FRAMEWORK_API extern String Saved;
			NX_FRAMEWORK_API extern String Temp;
		}

		namespace Debug
		{
			NX_FRAMEWORK_API extern Logger* Logs;
			NX_FRAMEWORK_API extern Stats* Statistiques;
			NX_FRAMEWORK_API extern Instruments* Instrumentor;
			NX_FRAMEWORK_API extern MemoryTracker* Memory;
		}

		NX_FRAMEWORK_API extern Arguments* Args;
		NX_FRAMEWORK_API extern Platform* PlatformTarget;
		NX_FRAMEWORK_API extern uint64 MainThreadId;

		NX_FRAMEWORK_API void Initialize(uint64 ArgC, char* ArgV[]);
		NX_FRAMEWORK_API void Shutdown();

		NX_FRAMEWORK_API void CreatePlatform();
		NX_FRAMEWORK_API void CreateArgs(uint64 ArgC, char* ArgV[]);
		NX_FRAMEWORK_API void DestroyArgs();
		NX_FRAMEWORK_API void CreatePathsAndFolders();
		NX_FRAMEWORK_API void DestroyTempFolder();
		NX_FRAMEWORK_API void CreateDebug(StringView Path);
		NX_FRAMEWORK_API void DestroyDebug();
	}
}
