#include "Core/NexusTestPch.h"

namespace NxTs
{
	TEST(Platform, Platform)
	{
		NxFr::Platform* Platform = NxFr::Platform::GetInstance();
		ASSERT_EQ(Platform->GetTarget(), NxFr::PlatformTarget::Windows);
	}
}
