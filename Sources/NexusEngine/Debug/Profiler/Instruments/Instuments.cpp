#include "Core/NexusEnginePch.h"
#include "Instuments.h"

#include "Debug/Profiler/Instruments/ChromeTracing.h"

#include "Core/NexusEngineGlobals.h"

namespace NxEn
{
	Instruments* Instruments::GetInstance()
	{
		return Globals::Instrumentor;
	}

	Instruments::Marker::Marker(StringView Text, Instruments* Target)
		: Text(Text), Target(Target)
	{
		Watch.Start();
	}

	Instruments::Marker::~Marker()
	{
		Watch.Stop();

		if (Target)
		{
			Target->Record(*this);
		}
	}

	Instruments* Instruments::Create(StringView Path, bool Start, Instruments::Tools Tool)
	{
		switch (Tool)
		{
		case NxEn::Instruments::Tools::ChromeTracing: return new ChromeTracing(Path, Start);
		}

		return nullptr;
	}

	void Instruments::Destroy(Instruments* Instance)
	{
		delete Instance;
	}

	void Instruments::Record(const Marker& Data)
	{
		if (!Recording)
		{
			return;
		}

		RecordMarker(Data);
	}

	void Instruments::StartRecording()
	{
		if (Recording)
		{
			NEXUS_LOG(Warning, Default, "Instruments is already recording");
			return;
		}

		Recording = true;
	}

	void Instruments::StopRecording()
	{
		if (!Recording)
		{
			NEXUS_LOG(Warning, Default, "Instruments is not recording");
			return;
		}

		Recording = false;
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
}
