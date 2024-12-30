#pragma once

#include "Core/NexusEngineBase.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"
#include "Debug/Stopwatch.h"

namespace NxEn
{
	class Instruments
	{
	public:
		enum class Tools
		{
			ChromeTracing
		};

		class Marker
		{
			friend class Instruments;

		public:
			NEXUS_ENGINE_API Marker(StringView Text, Instruments* Target);
			NEXUS_ENGINE_API ~Marker();

			const Stopwatch& GetWatch() const { return Watch; }
			StringView GetText() const { return Text; }

		private:
			Stopwatch Watch;
			StringView Text;
			Instruments* Target;
		};

	public:
		NEXUS_ENGINE_API static Instruments* Create(StringView Path, bool Start = false, Tools Tool = Tools::ChromeTracing);
		NEXUS_ENGINE_API static void Destroy(Instruments* Instance);

		NEXUS_ENGINE_API void StartRecording();
		NEXUS_ENGINE_API void StopRecording();

		NEXUS_ENGINE_API void Record(const Marker& Data);

		bool IsRecording() const { return Recording; }

		NEXUS_ENGINE_API static Instruments* GetInstance();

	protected:
		NEXUS_ENGINE_API Instruments(StringView Path, bool Start = false);
		NEXUS_ENGINE_API virtual ~Instruments();

		NEXUS_ENGINE_API virtual void RecordMarker(const Marker& Data) = 0;

		File Handle;
		String Buffer;
		bool Recording;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_INSTUMENT_LINE_INSTANCE(Instance, Name, Line) if (Instance) { ::NxEn::Instruments::Marker Marker##Line(Name, Instance); }
	#define NEXUS_INSTUMENT_SCOPE_INSTANCE(Instance, Name) NEXUS_INSTUMENT_LINE_INSTANCE(Instance, Name, NEXUS_LINE_NUMBER)
	#define NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance) NEXUS_INSTUMENT_SCOPE_INSTANCE(Instance, NEXUS_FUNCTION_SIGNATURE)

	#define NEXUS_INSTUMENT_LINE(Name, Line) NEXUS_INSTUMENT_LINE_INSTANCE(::NxEn::Instruments::GetInstance(), Name, Line)
	#define NEXUS_INSTUMENT_SCOPE(Name) NEXUS_INSTUMENT_SCOPE_INSTANCE(::NxEn::Instruments::GetInstance(), Name)
	#define NEXUS_INSTUMENT_FUNCTION() NEXUS_INSTUMENT_FUNCTION_INSTANCE(::NxEn::Instruments::GetInstance())
#elif NEXUS_DISTRIB
	#define NEXUS_INSTUMENT_LINE_INSTANCE(Name, Line, Instance)
	#define NEXUS_INSTUMENT_SCOPE_INSTANCE(Name, Instance)
	#define NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance)

	#define NEXUS_INSTUMENT_LINE(Name, Line)
	#define NEXUS_INSTUMENT_SCOPE(Name)
	#define NEXUS_INSTUMENT_FUNCTION()
#endif
