#pragma once

#if NEXUS_ENGINE_DLL_BUILD
#define NEXUS_ENGINE_API __declspec(dllexport)
#else
#define NEXUS_ENGINE_API __declspec(dllimport)
#endif

namespace NexusEngine
{
	NEXUS_ENGINE_API void ExecuteFromDll(const char* DllName, int Ordinal);

	NEXUS_ENGINE_API void WaitForUserCloseConsole();

	NEXUS_ENGINE_API void Log(const char* Message);

	NEXUS_ENGINE_API void LogEngine();
}