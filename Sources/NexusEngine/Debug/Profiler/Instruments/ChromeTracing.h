#pragma once

#include "Core/NexusEngineCore.h"
#include "Instumentor.h"

namespace NxEn
{
	class ChromeTracing : public Instruments
	{
		friend class Instruments;

	protected:
		NEXUS_ENGINE_API ChromeTracing(StringView Path, bool Start = false);
		NEXUS_ENGINE_API virtual ~ChromeTracing();

		NEXUS_ENGINE_API void RecordMarker(const Marker& Data) override;

	private:
		void WriteMarker(const Marker& Data);
		void WriteHeader();
		void WriteFooter();
		void Write();
	};
}

