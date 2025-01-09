#include "Core/NexusFrameworkPch.h"
#include "Platform.h"

#include "Platform/None/PlatformNone.h"
#include "Platform/Windows/PlatformWindows.h"

namespace NxFr
{
	Platform* Platform::GetInstance()
	{
#if NEXUS_WINDOWS
		static PlatformWindows Instance;
#else
		static PlatformNone Instance;
#endif
		return &Instance;
	}
}
