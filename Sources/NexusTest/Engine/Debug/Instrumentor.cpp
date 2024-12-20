#include "Core/NexusTestPch.h"

#include "Debug/Instrumentor.h"
#include "Misc/IO/Path.h"

namespace NxTs
{
	void Dummy(uint64 Count, NxEn::Instrument* Instrument)
	{
		NEXUS_INSTRUMENT_FUNCTION_INSTANCE(Instrument);

		for (uint64 Iteration = 0; Iteration < Count; Iteration++)
		{
			void* Dum = NxEn::Memory::Allocate(1024);
			NxEn::Memory::Free(Dum);
		}
	}

	void Function1(uint64 Count, NxEn::Instrument* Instrument)
	{
		NEXUS_INSTRUMENT_FUNCTION_INSTANCE(Instrument);

		Dummy(Count, Instrument);
	}

	void Function2(uint64 Count, NxEn::Instrument* Instrument)
	{
		NEXUS_INSTRUMENT_FUNCTION_INSTANCE(Instrument);

		Dummy(Count, Instrument);
		Function1(Count, Instrument);
	}

	TEST(Instrument, Instrument)
	{
		NxEn::Instrument Instrument(NxEn::Path::GetWorkingDirectory() + "Instrument.json");
		Instrument.StartRecording();

		{
			NEXUS_INSTRUMENT_FUNCTION_INSTANCE(&Instrument);

			Function1(1000, &Instrument);
			Function2(1000, &Instrument);
		}

		Instrument.StopRecording();
	}
}
