#include "Core/NexusTestPch.h"

#include "Debug/Profiler/Instruments/Instumentor.h"
#include "Misc/IO/Path.h"

namespace NxTs
{
	void Dummy(uint64 Count, NxEn::Instrumentor* Instrumentor)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instrumentor);

		for (uint64 Iteration = 0; Iteration < Count; Iteration++)
		{
			void* Dum = NxEn::Memory::Allocate(1024);
			NxEn::Memory::Free(Dum);
		}
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
		Function1(Count, Instrumentor);
	}

	TEST(Instrumentor, Instrumentor)
	{
		NxEn::Instrumentor* Instrumentor = NxEn::Instrumentor::Create(NxEn::Path::GetWorkingDirectory() + "Instruments.json");
		Instrumentor->StartRecording();

		{
			NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instrumentor);

			Function1(1000, Instrumentor);
			Function2(1000, Instrumentor);
		}

		Instrumentor->StopRecording();
		delete Instrumentor;
	}
}
