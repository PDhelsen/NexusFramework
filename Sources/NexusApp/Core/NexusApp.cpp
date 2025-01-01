#include "NexusApp.h"
#include "Memory/MemoryOperator.h"

#include "Core/NexusEngine.h"
#include "Core/NexusEditor.h"

#include "Core/NexusEngineEntryPoint.h"

namespace NxAp
{
	void HelloWorld()
	{
		NEXUS_LOG(Info, Default, "Hello World");
	}
}

int main()
{
	NxEn::Initialize();

	NxEd::HelloWorld();
	NxAp::HelloWorld();

	NxEn::Platform* Platform = NxEn::Platform::GetInstance();
	Platform->ExecuteFromDll("NexusSandbox", 1);
	Platform->WaitForUserToCloseTerminal();

	NxEn::Shutdown();
}
