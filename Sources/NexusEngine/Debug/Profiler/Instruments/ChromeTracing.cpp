#include "Core/NexusEnginePch.h"
#include "ChromeTracing.h"

#include "Application/Time.h"

namespace NxEn
{
	ChromeTracing::ChromeTracing(StringView Path, bool Start)
		: Instrumentor(Path, Start)
	{
		WriteHeader();
	}

	ChromeTracing::~ChromeTracing()
	{
		WriteFooter();
	}

	void ChromeTracing::RecordMarker(const InstrumentMarker& Data)
	{
		WriteMarker(Data);
	}

	void ChromeTracing::WriteMarker(const InstrumentMarker& Data)
	{
		// Expect timing in micro second
		StringUtility::Format(
			Buffer,
			StringView(",{\"cat\":\"function\",\"dur\":%.3f,\"name\":\"%s\",\"ph\":\"X\",\"pid\":0,\"tid\":0,\"ts\":%.3f}"),
			Data.GetWatch().GetElapsedTime(Time::SecondToMicro),
			Data.GetText().C(),
			Data.GetWatch().GetStartTime(Time::SecondToMicro)
		);

		Write();
	}

	void ChromeTracing::WriteHeader()
	{
		Buffer += "{\"otherData\": {},\"traceEvents\":[{}";
		Write();
	}

	void ChromeTracing::WriteFooter()
	{
		Buffer += "]}";
		Write();
	}

	void ChromeTracing::Write()
	{
		Handle.WriteText(Buffer);
		Buffer.Clear();
	}
}
