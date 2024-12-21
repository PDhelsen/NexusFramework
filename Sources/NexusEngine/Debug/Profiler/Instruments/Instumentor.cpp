#include "Core/NexusEnginePch.h"
#include "Instumentor.h"

#include "Debug/Profiler/Instruments/ChromeTracing.h"

namespace NxEn
{
	InstrumentMarker::InstrumentMarker(StringView Text, Instrumentor* Target)
		: Text(Text), Target(Target)
	{
		NEXUS_ASSERT(Target, "Target cannot be null");

		Watch.Start();
	}

	InstrumentMarker::~InstrumentMarker()
	{
		Watch.Stop();

		Target->Record(*this);
	}

	Instrumentor* Instrumentor::Create(StringView Path, bool Start, InstrumentTool Tool)
	{
		switch (Tool)
		{
		case NxEn::InstrumentTool::ChromeTracing: return new ChromeTracing(Path, Start);
		}

		return nullptr;
	}

	Instrumentor::Instrumentor(StringView Path, bool Start)
		: Handle(Path), Buffer(256), Recording(Start)
	{
		Handle.Delete();
		Handle.Create();
		Handle.Open(File::Mode::Append);
	}

	Instrumentor::~Instrumentor()
	{
		Handle.Close();
	}

	void Instrumentor::StartRecording()
	{
		NEXUS_ASSERT(!Recording, "Instrumentor is already recording");
		Recording = true;
	}

	void Instrumentor::StopRecording()
	{
		NEXUS_ASSERT(Recording, "Instrumentor is not recording");
		Recording = false;
	}

	void Instrumentor::Record(const InstrumentMarker& Data)
	{
		if (!Recording)
		{
			return;
		}

		RecordMarker(Data);
	}
}
