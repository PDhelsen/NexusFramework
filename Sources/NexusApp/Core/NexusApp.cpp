#include "NexusApp.h"

#include "Core/NexusEngineBase.h"
#include "Core/NexusEditor.h"

#include "Platform/Platform.h"

namespace NxAp
{
	void HelloWorld()
	{
		NEXUS_LOG(App, Info, 0, "Hello World")
	}
}

int main()
{
	NxEn::HelloWorld();
	NxEd::HelloWorld();
	NxAp::HelloWorld();

	NxEn::Platform* Platform = NxEn::Platform::GetInstance();
	Platform->ExecuteFromDll("NexusSandbox", 1);
	Platform->WaitForUserToCloseConsole();
}