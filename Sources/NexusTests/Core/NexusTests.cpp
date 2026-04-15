#include "NexusTests/Core/NexusTests.h"
#include "NexusFramework/Memory/MemoryOperator.h"

#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

namespace NxTs
{
	void Environment::SetUp()
	{
		NxFr::Initialize();
	}

	void Environment::TearDown()
	{
		NxFr::Shutdown();
	}
}

void NexusGTestMain()
{
	testing::AddGlobalTestEnvironment(new NxTs::Environment());
}
