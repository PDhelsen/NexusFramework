#include "Core/NexusTestPch.h"

#include "Debug/Profiler/Instruments/Instumentor.h"
#include "Misc/IO/Path.h"
#include "Platform/Platform.h"

namespace NxTs
{
	void Dummy(uint64 Count, NxEn::Instrumentor* Instrumentor)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instrumentor);

		NxEn::Platform::GetInstance()->Sleep(Count);
	}

	void Function1(uint64 Count, NxEn::Instrumentor* Instrumentor)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instrumentor);

		Dummy(Count, Instrumentor);
	}

	void Function2(uint64 Count, NxEn::Instrumentor* Instrumentor)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instrumentor);

		Dummy(Count, Instrumentor);
		Function1(Count * 2, Instrumentor);
	}

	TEST(Instrumentor, Instrumentor)
	{
		NxEn::Instrumentor* Instrumentor = NxEn::Instrumentor::Create(NxEn::Path::GetWorkingDirectory() + "Instruments.json");
		Instrumentor->StartRecording();

		{
			NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instrumentor);

			Function1(5, Instrumentor);
			Function2(10, Instrumentor);
		}

		Instrumentor->StopRecording();
		delete Instrumentor;
	}
}
