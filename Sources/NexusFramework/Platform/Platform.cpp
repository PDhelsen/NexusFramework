#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Platform/Platform.h"

#include "NexusFramework/Threading/Thread.h"

namespace NxFr
{
	Platform::Platform()
	{
	}

	Platform::~Platform()
	{
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
