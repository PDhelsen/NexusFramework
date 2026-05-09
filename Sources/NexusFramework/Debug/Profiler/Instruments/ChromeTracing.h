#pragma once

#include "NexusFramework/Debug/Profiler/Instruments/Instuments.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	class ChromeTracing : public Instruments
	{
	public:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, ChromeTracing)
		NEXUS_FRAMEWORK_API ChromeTracing(StringView Path, bool AutoStart = false, bool AutoFlush = false);
		NEXUS_FRAMEWORK_API virtual ~ChromeTracing();

	protected:
		NEXUS_FRAMEWORK_API void RecordMarker(const Marker& Data) override;
		NEXUS_FRAMEWORK_API void FlushMarkers() override;

	private:
		void WriteMarker(const Marker& Data);
		void WriteHeader();
		void WriteFooter();

	private:
		TextStream Stream;
		String Buffer;
	};
}

