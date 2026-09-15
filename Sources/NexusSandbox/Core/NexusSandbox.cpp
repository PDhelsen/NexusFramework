#include "NexusSandbox/Core/NexusSandbox.h"
#include "NexusFramework/Memory/Misc/Operator.h"


namespace NxSb
{
	void HelloWorld()
	{
		NX_LOG(Info, Default, "Hello World");
	}
}

int main(int argc, char* argv[])
{
	NxFr::Globals::Initialize(argc, argv);

	NxSb::HelloWorld();
	NxFr::Globals::PlatformTarget->WaitForUserToCloseTerminal();

	NxFr::Globals::Shutdown();
}
