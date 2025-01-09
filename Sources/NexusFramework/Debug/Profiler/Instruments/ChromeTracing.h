#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Instuments.h"

namespace NxEn
{
	class ChromeTracing : public Instruments
	{
		friend class Instruments;

	protected:
		NEXUS_FRAMEWORK_API ChromeTracing(StringView Path, bool Start = false);
		NEXUS_FRAMEWORK_API virtual ~ChromeTracing();

		NEXUS_FRAMEWORK_API void RecordMarker(const Marker& Data) override;

	private:
		void WriteMarker(const Marker& Data);
		void WriteHeader();
		void WriteFooter();
		void Write();
	};
}

