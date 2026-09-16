#include "NexusTests/Core/NexusTests.h"
#include "NexusFramework/Memory/Misc/Operator.h"

namespace NxTs
{
	void Environment::SetUp()
	{
		NxFr::Globals::Initialize(0, nullptr);
	}

	void Environment::TearDown()
	{
		NxFr::Globals::Shutdown();
	}
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	testing::AddGlobalTestEnvironment(new NxTs::Environment());
	return RUN_ALL_TESTS();
}
