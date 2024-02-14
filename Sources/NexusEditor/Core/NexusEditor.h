#pragma once

// Global defines, macro, function, ...

#if NEXUS_EDITOR_DLL_BUILD
	#define NEXUS_EDITOR_API __declspec(dllexport)
#else
	#define NEXUS_EDITOR_API __declspec(dllimport)
#endif

namespace NxEd
{
	NEXUS_EDITOR_API void HelloWorld();
}