#include "NexusSandbox.h"
#include "Memory/MemoryOperator.h"

#include "Core/NexusFramework.h"
#include "Core/NexusFrameworkEntryPoint.h"

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
