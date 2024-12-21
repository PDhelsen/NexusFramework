#pragma once

#include "Core/NexusEngineBase.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"
#include "Debug/Stopwatch.h"

#define NEXUS_DEFAULT_INSTRUMENTOR ::NxEn::InstrumentTool::ChromeTracing

namespace NxEn
{
	enum class InstrumentTool
	{
		ChromeTracing
	};

	class InstrumentMarker
	{
		friend class Instrumentor;

	public:
		NEXUS_ENGINE_API InstrumentMarker(StringView Text, Instrumentor* Target);
		NEXUS_ENGINE_API ~InstrumentMarker();

		const Stopwatch& GetWatch() const { return Watch; }
		StringView GetText() const { return Text; }

	private:
		Stopwatch Watch;
		StringView Text;
		Instrumentor* Target;
	};

	class Instrumentor
	{
	public:
		NEXUS_ENGINE_API static Instrumentor* Create(StringView Path, bool Start = false, InstrumentTool Tool = NEXUS_DEFAULT_INSTRUMENTOR);

		NEXUS_ENGINE_API Instrumentor(StringView Path, bool Start = false);
		NEXUS_ENGINE_API virtual ~Instrumentor();

		NEXUS_ENGINE_API void StartRecording();
		NEXUS_ENGINE_API void StopRecording();

		NEXUS_ENGINE_API void Record(const InstrumentMarker& Data);

		bool IsRecording() const { return Recording; }

		static Instrumentor* GetInstance() { static Instrumentor* Instance = Create(Path::GetWorkingDirectory() + "InstrumentSession.json"); return Instance; }

	protected:
		NEXUS_ENGINE_API virtual void RecordMarker(const InstrumentMarker& Data) = 0;

		File Handle;
		String Buffer;
		bool Recording;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_INSTUMENT_LINE_INSTANCE(Name, Line, Instance) NxEn::InstrumentMarker Marker##Line(Name, Instance)
#define NEXUS_INSTUMENT_SCOPE_INSTANCE(Name, Instance) NEXUS_INSTUMENT_LINE_INSTANCE(Name, NEXUS_LINE_NUMBER, Instance)
#define NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance) NEXUS_INSTUMENT_SCOPE_INSTANCE(NEXUS_FUNCTION_SIGNATURE, Instance)

#define NEXUS_INSTUMENT_LINE(Name, Line) NEXUS_INSTUMENT_LINE_INSTANCE(Name, Line, NxEn::Instrumentor::GetInstance())
#define NEXUS_INSTUMENT_SCOPE(Name) NEXUS_INSTUMENT_SCOPE_INSTANCE(Name, NxEn::Instrumentor::GetInstance())
#define NEXUS_INSTUMENT_FUNCTION() NEXUS_INSTUMENT_FUNCTION_INSTANCE(NxEn::Instrumentor::GetInstance())
#elif NEXUS_DISTRIB
#define NEXUS_INSTUMENT_LINE_INSTANCE(Name, Line, Instance)
#define NEXUS_INSTUMENT_SCOPE_INSTANCE(Name, Instance)
#define NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance)

#define NEXUS_INSTUMENT_LINE(Name, Line)
#define NEXUS_INSTUMENT_SCOPE(Name)
#define NEXUS_INSTUMENT_FUNCTION()
#endif
