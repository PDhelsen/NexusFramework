#include "NexusSandbox/Core/NexusSandbox.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxSb
{
	void HelloWorld()
	{
		NEXUS_LOG(Info, Default, "Hello World");
	}
}

int main()
{
	NxFr::Initialize();

	NxSb::HelloWorld();
	NxFr::Platform::GetInstance()->WaitForUserToCloseTerminal();

	NxFr::Shutdown();
}
