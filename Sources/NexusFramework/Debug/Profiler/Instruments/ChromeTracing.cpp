#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Instruments/ChromeTracing.h"

#include "NexusFramework/Time/Time.h"

namespace NxFr
{
	ChromeTracing::ChromeTracing(StringView Path, bool AutoStart, bool AutoFlush)
		: Instruments(AutoStart, AutoFlush), Stream(Path), Buffer(256)
	{
		Stream.Open(File::Mode::Write);

		WriteHeader();
	}

	ChromeTracing::~ChromeTracing()
	{
		WriteFooter();

		Stream.Close();
	}

	void ChromeTracing::RecordMarker(const Marker& Data)
	{
		WriteMarker(Data);
	}

	void ChromeTracing::FlushMarkers()
	{
		Stream.Flush();
	}

	void ChromeTracing::WriteMarker(const Marker& Data)
	{
		// Expect timing in micro second
		Buffer.Format(
			StringView(",{\"cat\":\"function\",\"dur\":%.3f,\"name\":\"%s\",\"ph\":\"X\",\"pid\":0,\"tid\":%d,\"ts\":%.3f}"),
			Data.GetWatch().GetElapsedTime(Time::SecondToMicro),
			Data.GetText().C(),
			Thread::IsMainThread() ? 0 : Thread::ThreadId(),
			Data.GetWatch().GetStartTime(Time::SecondToMicro)
		);

		Stream.WriteBlock(Buffer);
	}

	void ChromeTracing::WriteHeader()
	{
		Stream.WriteBlock("{\"otherData\": {},\"traceEvents\":[{}");
	}

	void ChromeTracing::WriteFooter()
	{
		Stream.WriteBlock("]}");
	}
}
