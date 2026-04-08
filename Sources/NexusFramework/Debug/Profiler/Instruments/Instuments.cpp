#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Instruments/Instuments.h"

#include "NexusFramework/Debug/Profiler/Instruments/ChromeTracing.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
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

	Instruments* Instruments::Create(StringView Path, bool AutoStart, bool AutoFlush, Instruments::Tools Tool)
	{
		switch (Tool)
		{
		case NxFr::Instruments::Tools::ChromeTracing: return new ChromeTracing(Path, AutoStart, AutoFlush);
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

		Lock LockGuard(Guard);
		RecordMarker(Data);
		if (AutoFlush)
		{
			FlushMarkers();
		}
	}

	void Instruments::Flush()
	{
		FlushMarkers();
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

	Instruments::Instruments(bool AutoStart, bool AutoFlush)
		: Recording(AutoStart), AutoFlush(AutoFlush)
	{
		
	}

	Instruments::~Instruments()
	{
	}
}
