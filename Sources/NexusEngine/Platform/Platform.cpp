#include "Core/NexusEnginePch.h"
#include "Platform.h"

#include "Platform/None/PlatformNone.h"
#include "Platform/Windows/PlatformWindows.h"

namespace NxEn
{
	Platform* Platform::Instance = Platform::Create();

	Platform* Platform::Create()
	{
#if NEXUS_WINDOWS
		return new PlatformWindows();
#else
		return new PlatformNone();
#endif
	}
}