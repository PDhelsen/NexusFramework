#pragma once

#include "Types/Types.h"

#if NEXUS_ENGINE_DLL_BUILD
#define NEXUS_ENGINE_API __declspec(dllexport)
#else
#define NEXUS_ENGINE_API __declspec(dllimport)
#endif

namespace NxEn
{
	NEXUS_ENGINE_API void ExecuteFromDll(const char* DllName, int8 Ordinal);

	NEXUS_ENGINE_API void WaitForUserCloseConsole();

	NEXUS_ENGINE_API void HelloWorld();
}