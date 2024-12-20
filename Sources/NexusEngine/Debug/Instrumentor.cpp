#include "Core/NexusEnginePch.h"
#include "Instrumentor.h"

namespace NxEn
{
	Instrument::Marker::Marker(StringView Name, Instrument* Target)
		: Name(Name), Target(Target)
	{
		NEXUS_ASSERT(Target, "Target cannot be null");

		Watch.Start();
	}

	Instrument::Marker::~Marker()
	{
		Watch.Stop();

		Target->Record(*this);
	}

	Instrument::Instrument(StringView Path, bool AutoRecord)
		: Handle(Path), Buffer(256), Recording(AutoRecord)
	{
		Handle.Delete();
		Handle.Create();
		Handle.Open(File::Mode::Append);

		WriteHeader();
	}

	Instrument::~Instrument()
	{
		WriteFooter();

		Handle.Close();
	}

	void Instrument::StartRecording()
	{
		NEXUS_ASSERT(!Recording, "Instrument is already recording");
		Recording = true;
	}

	void Instrument::StopRecording()
	{
		NEXUS_ASSERT(Recording, "Instrument is not recording");
		Recording = false;
	}

	void Instrument::Record(const Marker& Data)
	{
		if (!Recording)
		{
			return;
		}

		WriteMarker(Data);
	}

	void Instrument::WriteMarker(const Marker& Data)
	{
		// Expect timing in micro second
		StringUtility::Format(
			Buffer,
			StringView(",{\"cat\":\"function\",\"dur\":%.3f,\"name\":\"%s\",\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":%.3f}"),
			Data.Watch.GetElapsedTime(1000000),
			Data.Name.C(),
			Data.Watch.GetStartTime(1000000)
		);

		Write();
	}

	void Instrument::WriteHeader()
	{
		Buffer += "{\"otherData\": {},\"traceEvents\":[{}";
		Write();
	}

	void Instrument::WriteFooter()
	{
		Buffer += "]}";
		Write();
	}

	void Instrument::Write()
	{
		Handle.WriteText(Buffer);
		Buffer.Clear();
	}
}
