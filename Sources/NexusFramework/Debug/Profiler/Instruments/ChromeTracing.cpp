#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Instruments/ChromeTracing.h"

#include "NexusFramework/Time/Time.h"

namespace NxFr
{
	ChromeTracing::ChromeTracing(StringView Path, bool Start)
		: Instruments(Path, Start)
	{
		WriteHeader();
	}

	ChromeTracing::~ChromeTracing()
	{
		WriteFooter();
	}

	void ChromeTracing::RecordMarker(const Marker& Data)
	{
		WriteMarker(Data);
	}

	void ChromeTracing::WriteMarker(const Marker& Data)
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
