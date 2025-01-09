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
	NxEn::Initialize();

	NxSb::HelloWorld();
	NxEn::Platform::GetInstance()->WaitForUserToCloseTerminal();

	NxEn::Shutdown();
}
