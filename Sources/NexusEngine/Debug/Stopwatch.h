#pragma once

#include "Core/NexusEngine.h"

namespace NxEn
{
	class Stopwatch
	{
	public:
		NEXUS_ENGINE_API Stopwatch(bool AutoStart = false);
		NEXUS_ENGINE_API ~Stopwatch();

		NEXUS_ENGINE_API void Start();
		NEXUS_ENGINE_API double Pause(double Unit = 1.0);
		NEXUS_ENGINE_API void Resume();
		NEXUS_ENGINE_API double Stop(double Unit = 1.0);
		NEXUS_ENGINE_API void Reset();

		NEXUS_ENGINE_API double GetElapsedTime(double Unit = 1.0);

		bool IsStarted() const { return Started; }
		bool IsPaused() const { return Paused; }

	private:
		void ComputeElapsed();

		double StartTimer;
		double ElapsedTime;
		bool Started;
		bool Paused;
	};
}
