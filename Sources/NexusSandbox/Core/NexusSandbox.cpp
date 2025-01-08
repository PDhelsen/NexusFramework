#include "NexusSandbox.h"
#include "Memory/MemoryOperator.h"

#include "Core/NexusEngine.h"
#include "Core/NexusEngineEntryPoint.h"

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
