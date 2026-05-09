#include "NexusSandbox/Core/NexusSandbox.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusFramework/Core/NexusFramework.h"
#include "NexusFramework/Core/NexusFrameworkCommon.h"

namespace NxSb
{
	void HelloWorld()
	{
		NEXUS_LOG(Info, Default, "Hello World");
	}
}

int main(int argc, char* argv[])
{
	NxFr::Initialize(argc, argv);

	NxSb::HelloWorld();
	NxFr::Globals::PlatformTarget->WaitForUserToCloseTerminal();

	NxFr::Shutdown();
}
