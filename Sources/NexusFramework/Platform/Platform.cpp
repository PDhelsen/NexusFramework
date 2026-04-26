#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Platform/Platform.h"

#include "NexusFramework/Platform/None/PlatformNone.h"
#include "NexusFramework/Platform/Windows/PlatformWindows.h"

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

	void Platform::ClearDll()
	{
		for (auto& [Name, Ptr] : Dlls)
		{
			UnloadDll(Name);
		}
	}

	void Platform::ThreadRun(Thread* Instance) const
	{
		Instance->RunOnThread();
	}
}
