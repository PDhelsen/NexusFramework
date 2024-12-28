#include "Core/NexusEnginePch.h"
#include "Instumentor.h"

#include "Debug/Profiler/Instruments/ChromeTracing.h"

namespace NxEn
{
	Instruments* Instruments::GetInstance()
	{
		AllocatorContext Context(nullptr);
		static Instruments* Instance = Create(Path::GetWorkingDirectory() + "InstrumentSession.json");
		return Instance;
	}

	Instruments::Marker::Marker(StringView Text, Instruments* Target)
		: Text(Text), Target(Target)
	{
		NEXUS_ASSERT(Target, "Target cannot be null");

		Watch.Start();
	}

	Instruments::Marker::~Marker()
	{
		Watch.Stop();

		Target->Record(*this);
	}

	Instruments* Instruments::Create(StringView Path, bool Start, Instruments::Tools Tool)
	{
		switch (Tool)
		{
		case NxEn::Instruments::Tools::ChromeTracing: return new ChromeTracing(Path, Start);
		}

		return nullptr;
	}

	Instruments::Instruments(StringView Path, bool Start)
		: Handle(Path), Buffer(256), Recording(Start)
	{
		Handle.Delete();
		Handle.Create();
		Handle.Open(File::Mode::Append);
	}

	Instruments::~Instruments()
	{
		Handle.Close();
	}

	void Instruments::StartRecording()
	{
		NEXUS_ASSERT(!Recording, "Instruments is already recording");
		Recording = true;
	}

	void Instruments::StopRecording()
	{
		NEXUS_ASSERT(Recording, "Instruments is not recording");
		Recording = false;
	}

	void Instruments::Record(const Marker& Data)
	{
		if (!Recording)
		{
			return;
		}

		RecordMarker(Data);
	}
}
