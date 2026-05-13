#pragma once

#include "NexusFramework/External/GTest.h"

namespace NxTs
{
	class Environment : public ::testing::Environment
	{
	public:
		void SetUp() override;
		void TearDown() override;
	};
}
