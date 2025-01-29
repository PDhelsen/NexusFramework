#pragma once

#if NEXUS_FRAMEWORK_DLL
	#if NEXUS_FRAMEWORK_DLL_BUILD
		#define NEXUS_FRAMEWORK_API __declspec(dllexport)
	#else
		#define NEXUS_FRAMEWORK_API __declspec(dllimport)
	#endif
#else
		#define NEXUS_FRAMEWORK_API
#endif
