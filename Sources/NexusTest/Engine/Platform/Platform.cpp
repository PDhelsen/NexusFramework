#include <gtest/gtest.h>

#include "Platform/Platform.h"

namespace NxTs
{
	TEST(Platform, Platform)
	{
		NxEn::Platform* Platform = NxEn::Platform::GetInstance();
		ASSERT_EQ(Platform->GetTarget(), NxEn::PlatformTarget::Windows);
	}
}
