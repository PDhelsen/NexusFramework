#pragma once

#if NEXUS_ENGINE_DLL_BUILD
	#define NEXUS_ENGINE_API __declspec(dllexport)
#else
	#define NEXUS_ENGINE_API __declspec(dllimport)
#endif

namespace NxEn
{
	NEXUS_ENGINE_API void HelloWorld();

	NEXUS_ENGINE_API bool Initialize();
	NEXUS_ENGINE_API bool Shutdown();
}
