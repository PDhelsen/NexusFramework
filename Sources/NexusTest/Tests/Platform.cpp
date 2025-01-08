#include "Core/NexusTestPch.h"

namespace NxTs
{
	TEST(Platform, Platform)
	{
		NxEn::Platform* Platform = NxEn::Platform::GetInstance();
		ASSERT_EQ(Platform->GetTarget(), NxEn::PlatformTarget::Windows);
	}
}
