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
	return NxFr::Globals::RunUnitTest<NxTs::Environment>(argc, argv);
}
