#include "Core/NexusEnginePch.h"
#include "Platform.h"

#include "Platform/None/PlatformNone.h"
#include "Platform/Windows/PlatformWindows.h"

namespace NxEn
{
	Platform* Platform::GetInstance()
	{
		AllocatorContext Context(nullptr);
#if NEXUS_WINDOWS
		static Platform* Instance = new PlatformWindows();
#else
		static Platform* Instance = new PlatformNone();
#endif
		return Instance;
	}
}
