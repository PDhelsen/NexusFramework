#pragma once

#include "NexusFramework/External/GTest.h"
#include "NexusFramework/Core/NexusFramework.h"

namespace NxTs
{
	class Environment : public ::testing::Environment
	{
	public:
		void SetUp() override;
		void TearDown() override;
	};
}
