#pragma once

#include "NexusFramework/Core/NexusFramework.h"

namespace NxTs
{
	class Environment : public NxFr::UnitTestEnvironment
	{
	public:
		void SetUp() override;
		void TearDown() override;
	};
}
