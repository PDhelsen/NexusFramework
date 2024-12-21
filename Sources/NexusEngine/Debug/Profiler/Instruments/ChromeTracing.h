#pragma once

#include "Core/NexusEngineBase.h"
#include "Instumentor.h"

namespace NxEn
{
	class ChromeTracing : public Instrumentor
	{
		friend class Instrumentor;

	protected:
		NEXUS_ENGINE_API ChromeTracing(StringView Path, bool Start = false);
		NEXUS_ENGINE_API virtual ~ChromeTracing();

		NEXUS_ENGINE_API void RecordMarker(const InstrumentMarker& Data) override;

	private:
		void WriteMarker(const InstrumentMarker& Data);
		void WriteHeader();
		void WriteFooter();
		void Write();
	};
}

