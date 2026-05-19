#pragma once

#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Time/Stopwatch.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	class NX_FRAMEWORK_API Instruments
	{
	public:
		class NX_FRAMEWORK_API Scope
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

		NX_NOCOPY_NOMOVE(Instruments)
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

	private:
		bool Recording;
		bool AutoFlush;

		Mutex Guard;
	};
}

#if NX_DEBUG || NX_RELEASE
	#define NX_INSTUMENT_LINE_INSTANCE(Instance, Name, Line) ::NxFr::Instruments::Scope Marker##Line(Name, Instance);
	#define NX_INSTUMENT_SCOPE_INSTANCE(Instance, Name) NX_INSTUMENT_LINE_INSTANCE(Instance, Name, NX_LINE_NUMBER)
	#define NX_INSTUMENT_FUNCTION_INSTANCE(Instance) NX_INSTUMENT_SCOPE_INSTANCE(Instance, NX_FUNCTION_SIGNATURE)

	#define NX_INSTUMENT_LINE(Name, Line) NX_INSTUMENT_LINE_INSTANCE(::NxFr::Globals::Debug::Instrumentor, Name, Line)
	#define NX_INSTUMENT_SCOPE(Name) NX_INSTUMENT_SCOPE_INSTANCE(::NxFr::Globals::Debug::Instrumentor, Name)
	#define NX_INSTUMENT_FUNCTION() NX_INSTUMENT_FUNCTION_INSTANCE(::NxFr::Globals::Debug::Instrumentor)
#elif NX_DISTRIB
	#define NX_INSTUMENT_LINE_INSTANCE(Name, Line, Instance)
	#define NX_INSTUMENT_SCOPE_INSTANCE(Name, Instance)
	#define NX_INSTUMENT_FUNCTION_INSTANCE(Instance)

	#define NX_INSTUMENT_LINE(Name, Line)
	#define NX_INSTUMENT_SCOPE(Name)
	#define NX_INSTUMENT_FUNCTION()
#endif
