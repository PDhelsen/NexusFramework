#pragma once

#include "Core/NexusEngineBase.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"
#include "Debug/Stopwatch.h"

namespace NxEn
{
	class Instrument
	{
	public:
		struct Marker
		{
			Stopwatch Watch;
			StringView Name;
			Instrument* Target;

			NEXUS_ENGINE_API Marker(StringView Name, Instrument* Target);
			NEXUS_ENGINE_API ~Marker();
		};

		NEXUS_ENGINE_API Instrument(StringView Path, bool AutoRecord = false);
		NEXUS_ENGINE_API ~Instrument();

		NEXUS_ENGINE_API void StartRecording();
		NEXUS_ENGINE_API void StopRecording();

		NEXUS_ENGINE_API void Record(const Marker& Data);

		bool IsRecording() const { return Recording; }

		static Instrument* GetInstance() { static Instrument* Instance = new Instrument(Path::GetWorkingDirectory() + "Instrument.json"); return Instance; }
		
	private:
		void WriteMarker(const Marker& Data);
		void WriteHeader();
		void WriteFooter();
		void Write();

		File Handle;
		String Buffer;
		bool Recording;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_INSTRUMENT_LINE_INSTANCE(Name, Line, Instance) NxEn::Instrument::Marker Marker##Line(Name, Instance)
#define NEXUS_INSTRUMENT_SCOPE_INSTANCE(Name, Instance) NEXUS_INSTRUMENT_LINE_INSTANCE(Name, NEXUS_LINE_NUMBER, Instance)
#define NEXUS_INSTRUMENT_FUNCTION_INSTANCE(Instance) NEXUS_INSTRUMENT_SCOPE_INSTANCE(NEXUS_FUNCTION_SIGNATURE, Instance)

#define NEXUS_INSTRUMENT_LINE(Name, Line) NEXUS_INSTRUMENT_LINE_INSTANCE(Name, Line, NxEn::Instrument::GetInstance())
#define NEXUS_INSTRUMENT_SCOPE(Name) NEXUS_INSTRUMENT_SCOPE_INSTANCE(Name, NxEn::Instrument::GetInstance())
#define NEXUS_INSTRUMENT_FUNCTION() NEXUS_INSTRUMENT_FUNCTION_INSTANCE(NxEn::Instrument::GetInstance())
#elif NEXUS_DISTRIB
#define NEXUS_INSTRUMENT_LINE_INSTANCE(Name, Line, Instance)
#define NEXUS_INSTRUMENT_SCOPE_INSTANCE(Name, Instance)
#define NEXUS_INSTRUMENT_FUNCTION_INSTANCE(Instance)

#define NEXUS_INSTRUMENT_LINE(Name, Line)
#define NEXUS_INSTRUMENT_SCOPE(Name)
#define NEXUS_INSTRUMENT_FUNCTION()
#endif
