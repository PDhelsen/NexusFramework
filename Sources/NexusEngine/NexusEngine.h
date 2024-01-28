#pragma once

#if NEXUS_ENGINE_DLL_BUILD
#define NEXUS_ENGINE_API __declspec(dllexport)
#else
#define NEXUS_ENGINE_API __declspec(dllimport)
#endif

namespace NexusEngine
{
	NEXUS_ENGINE_API void LogEngine();
}