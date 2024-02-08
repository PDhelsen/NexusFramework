#include "Debug/Logger.h"

#include "NexusEngine.h"
#include "NexusEditor.h"

#include "NexusApp.h"

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

	// TODO: Handle Different dll name and functions
	NxEn::ExecuteFromDll("NexusSandbox", 1);

	NxEn::WaitForUserCloseConsole();
}