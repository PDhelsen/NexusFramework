#include "Core/NexusPch.h"
#include "Platform.h"

#include "Platform/PlatformWindows.h"

namespace NxEn
{
	Platform* Platform::Instance = Platform::Create();

	Platform* Platform::Create()
	{
#if NEXUS_WINDOWS
		return new PlatformWindows();
#endif
		
		return new Platform();
	}
}