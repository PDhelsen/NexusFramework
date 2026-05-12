#pragma once

#include "NexusFramework/Debug/Profiler/Instruments/Instuments.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API ChromeTracing : public Instruments
	{
	public:
		NEXUS_NOCOPY_NOMOVE(ChromeTracing)
		ChromeTracing(StringView Path, bool AutoStart = false, bool AutoFlush = false);
		virtual ~ChromeTracing();

	protected:
		void RecordMarker(const Marker& Data) override;
		void FlushMarkers() override;

	private:
		void WriteMarker(const Marker& Data);
		void WriteHeader();
		void WriteFooter();

	private:
		TextStream Stream;
		String Buffer;
	};
}

