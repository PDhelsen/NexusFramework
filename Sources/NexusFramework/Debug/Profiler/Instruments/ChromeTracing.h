#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Debug/Profiler/Instruments/Instuments.h"

namespace NxFr
{
	class ChromeTracing : public Instruments
	{
		friend class Instruments;

	protected:
		NEXUS_FRAMEWORK_API ChromeTracing(StringView Path, bool AutoStart = false, bool AutoFlush = false);
		NEXUS_FRAMEWORK_API virtual ~ChromeTracing();

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

