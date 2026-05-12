#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	struct NEXUS_FRAMEWORK_API Stopwatch
	{
	public:
		Stopwatch(bool AutoStart = false);

		void Start();
		double Pause(double Unit = 1.0);
		void Resume();
		double Stop(double Unit = 1.0);
		void Reset();
		double Peek(double Unit = 1.0) const;

		double GetStartTime(double Unit = 1.0) const;
		double GetElapsedTime(double Unit = 1.0) const;

		bool IsStarted() const { return Started; }
		bool IsPaused() const { return Paused; }

	private:
		double StartTimer;
		double ElapsedTime;
		bool Started;
		bool Paused;
	};
}
