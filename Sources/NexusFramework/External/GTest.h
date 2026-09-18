#pragma once

#pragma warning(push)
#pragma warning(disable: 26439)
#pragma warning(disable: 26495)

#include <gtest/gtest.h>

namespace NxFr
{
	using UnitTestEnvironment = ::testing::Environment;

	namespace Globals
	{
		template<typename T>
		int RunUnitTest(int ArgC, char* ArgV[])
		{
			::testing::InitGoogleTest(&ArgC, ArgV);
			::testing::AddGlobalTestEnvironment(new T());
			return RUN_ALL_TESTS();
		}
	}
}

#pragma warning(pop)
