#pragma once

#include "Core/NexusFrameworkCore.h"

namespace NxEn
{
	class Stopwatch
	{
	public:
		NEXUS_FRAMEWORK_API Stopwatch(bool AutoStart = false);
		NEXUS_FRAMEWORK_API ~Stopwatch();

		NEXUS_FRAMEWORK_API void Start();
		NEXUS_FRAMEWORK_API double Pause(double Unit = 1.0);
		NEXUS_FRAMEWORK_API void Resume();
		NEXUS_FRAMEWORK_API double Stop(double Unit = 1.0);
		NEXUS_FRAMEWORK_API void Reset();
		NEXUS_FRAMEWORK_API double Peek(double Unit = 1.0);

		NEXUS_FRAMEWORK_API double GetStartTime(double Unit = 1.0) const;
		NEXUS_FRAMEWORK_API double GetElapsedTime(double Unit = 1.0) const;

		bool IsStarted() const { return Started; }
		bool IsPaused() const { return Paused; }

	private:
		inline double Now() const;

		double StartTimer;
		double ElapsedTime;
		bool Started;
		bool Paused;
	};
}
