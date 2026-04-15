#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Platform, Platform)
	{
		NxFr::Platform* Platform = NxFr::Platform::GetInstance();
		ASSERT_EQ(Platform->GetTarget(), NxFr::PlatformTarget::Windows);
	}

	TEST(Platform, WorkingDir)
	{
		NxFr::Platform* Platform = NxFr::Platform::GetInstance();

		NxFr::String WorkingDir = NxFr::Path::GetWorkingDirectory();
		NxFr::String UpdatedWorkingDir = NxFr::Path::Resolve(NxFr::Path::Combine(WorkingDir, "../"));

		Platform->SetWorkingDirectory(UpdatedWorkingDir);
		ASSERT_EQ(Platform->GetWorkingDirectory(), UpdatedWorkingDir);

		Platform->SetWorkingDirectory(WorkingDir);
	}
}
