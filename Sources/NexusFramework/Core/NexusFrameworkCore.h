#pragma once

#if NEXUS_FRAMEWORK_DLL
	#define NEXUS_FRAMEWORK_API __declspec(dllexport)
#else
	#define NEXUS_FRAMEWORK_API __declspec(dllimport)
#endif
