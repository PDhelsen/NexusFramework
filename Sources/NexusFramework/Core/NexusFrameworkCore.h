#pragma once

#if NX_FRAMEWORK_DLL
	#define NX_FRAMEWORK_API __declspec(dllexport)
#else
	#define NX_FRAMEWORK_API __declspec(dllimport)
#endif
