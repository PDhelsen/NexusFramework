#include "Core/NexusEnginePch.h"
#include "Platform.h"

#include "Platform/None/PlatformNone.h"
#include "Platform/Windows/PlatformWindows.h"

namespace NxEn
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
