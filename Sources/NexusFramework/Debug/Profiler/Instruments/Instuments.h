#pragma once

#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Time/Stopwatch.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API Instruments
	{
	public:
		class NEXUS_FRAMEWORK_API Scope
		{
			friend class Instruments;

		public:
			Scope(StringView Text, Instruments* Target);
			~Scope();

			const Stopwatch& GetWatch() const { return Watch; }
			StringView GetText() const { return Text; }

		private:
			Stopwatch Watch;
			StringView Text;
			Instruments* Target;
		};

	public:
		NEXUS_NOCOPY_NOMOVE(Instruments)
		Instruments(bool AutoStart = false, bool AutoFlush = false);
		virtual ~Instruments();

		void Record(const Scope& Data);
		void Flush();

		void StartRecording();
		void StopRecording();

		bool IsRecording() const { return Recording; }
		bool GetAutoFlush() const { return AutoFlush; }
		void SetAutoFlush(bool Auto) { AutoFlush = true; }

	protected:
		virtual void RecordMarker(const Scope& Data) = 0;
		virtual void FlushMarkers() = 0;

		bool Recording;
		bool AutoFlush;

		Mutex Guard;
	};
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_INSTUMENT_LINE_INSTANCE(Instance, Name, Line) ::NxFr::Instruments::Scope Marker##Line(Name, Instance);
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
