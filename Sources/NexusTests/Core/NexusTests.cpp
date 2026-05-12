#include "NexusTests/Core/NexusTests.h"
#include "NexusFramework/Memory/Misc/Operator.h"

#include "NexusFramework/Core/NexusFrameworkCommon.h"

namespace NxTs
{
	void Environment::SetUp()
	{
		NxFr::Initialize(0, nullptr);
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
