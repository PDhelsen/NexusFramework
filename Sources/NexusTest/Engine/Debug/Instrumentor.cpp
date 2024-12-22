#include "Core/NexusTestPch.h"

#include "Debug/Profiler/Instruments/Instumentor.h"
#include "Misc/IO/Path.h"
#include "Platform/Platform.h"

namespace NxTs
{
	void Dummy(uint64 Count, NxEn::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		NxEn::Platform::GetInstance()->Sleep(Count);
	}

	void Function1(uint64 Count, NxEn::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		Dummy(Count, Instruments);
	}

	void Function2(uint64 Count, NxEn::Instruments* Instruments)
	{
		NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

		Dummy(Count, Instruments);
		Function1(Count * 2, Instruments);
	}

	TEST(Instruments, Instruments)
	{
		NxEn::Instruments* Instruments = NxEn::Instruments::Create(NxEn::Path::GetWorkingDirectory() + "Instruments.json");
		Instruments->StartRecording();

		{
			NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instruments);

			Function1(5, Instruments);
			Function2(10, Instruments);
		}

		Instruments->StopRecording();
		delete Instruments;
	}
}
