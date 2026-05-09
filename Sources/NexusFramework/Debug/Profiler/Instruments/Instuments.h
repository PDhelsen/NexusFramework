#pragma once

#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Time/Stopwatch.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	class Instruments
	{
	public:
		class Marker
		{
			friend class Instruments;

		public:
			NEXUS_FRAMEWORK_API Marker(StringView Text, Instruments* Target);
			NEXUS_FRAMEWORK_API ~Marker();

			const Stopwatch& GetWatch() const { return Watch; }
			StringView GetText() const { return Text; }

		private:
			Stopwatch Watch;
			StringView Text;
			Instruments* Target;
		};

	public:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, Instruments)
		NEXUS_FRAMEWORK_API Instruments(bool AutoStart = false, bool AutoFlush = false);
		NEXUS_FRAMEWORK_API virtual ~Instruments();

		NEXUS_FRAMEWORK_API void Record(const Marker& Data);
		NEXUS_FRAMEWORK_API void Flush();

		NEXUS_FRAMEWORK_API void StartRecording();
		NEXUS_FRAMEWORK_API void StopRecording();

		bool IsRecording() const { return Recording; }
		bool GetAutoFlush() const { return AutoFlush; }
		void SetAutoFlush(bool Auto) { AutoFlush = true; }

	protected:
		NEXUS_FRAMEWORK_API virtual void RecordMarker(const Marker& Data) = 0;
		NEXUS_FRAMEWORK_API virtual void FlushMarkers() = 0;

		bool Recording;
		bool AutoFlush;

		Mutex Guard;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_INSTUMENT_LINE_INSTANCE(Instance, Name, Line) ::NxFr::Instruments::Marker Marker##Line(Name, Instance);
	#define NEXUS_INSTUMENT_SCOPE_INSTANCE(Instance, Name) NEXUS_INSTUMENT_LINE_INSTANCE(Instance, Name, NEXUS_LINE_NUMBER)
	#define NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance) NEXUS_INSTUMENT_SCOPE_INSTANCE(Instance, NEXUS_FUNCTION_SIGNATURE)

	#define NEXUS_INSTUMENT_LINE(Name, Line) NEXUS_INSTUMENT_LINE_INSTANCE(::NxFr::Globals::Instrumentor, Name, Line)
	#define NEXUS_INSTUMENT_SCOPE(Name) NEXUS_INSTUMENT_SCOPE_INSTANCE(::NxFr::Globals::Instrumentor, Name)
	#define NEXUS_INSTUMENT_FUNCTION() NEXUS_INSTUMENT_FUNCTION_INSTANCE(::NxFr::Globals::Instrumentor)
#elif NEXUS_DISTRIB
	#define NEXUS_INSTUMENT_LINE_INSTANCE(Name, Line, Instance)
	#define NEXUS_INSTUMENT_SCOPE_INSTANCE(Name, Instance)
	#define NEXUS_INSTUMENT_FUNCTION_INSTANCE(Instance)

	#define NEXUS_INSTUMENT_LINE(Name, Line)
	#define NEXUS_INSTUMENT_SCOPE(Name)
	#define NEXUS_INSTUMENT_FUNCTION()
#endif
