#include "Core/NexusFrameworkPch.h"
#include "Stopwatch.h"

#include "Platform/Platform.h"

namespace NxFr
{
	Stopwatch::Stopwatch(bool AutoStart)
		: StartTimer(0.0), ElapsedTime(0.0), Started(false), Paused(false)
	{
		if (AutoStart)
		{
			Start();
		}
	}

	Stopwatch::~Stopwatch()
	{
	}

	void Stopwatch::Start()
	{
		if (Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was already started");
			return;
		}

		StartTimer = Now();
		ElapsedTime = 0.0;
		Started = true;
		Paused = false;
	}

	double Stopwatch::Pause(double Unit)
	{
		if (Paused)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was already paused");
			return 0.0;
		}

		if (!Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return 0.0;
		}

		ElapsedTime += Now() - StartTimer;
		Paused = true;

		return GetElapsedTime(Unit);
	}

	void Stopwatch::Resume()
	{
		if (!Paused)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not paused");
			return;
		}

		if (!Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return;
		}

		StartTimer = Platform::GetInstance()->GetProcessorTimer();
		Paused = false;
	}

	double Stopwatch::Stop(double Unit)
	{
		if (!Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return 0.0;
		}

		ElapsedTime += Paused ? 0 : Now() - StartTimer;
		Started = false;
		Paused = false;

		return GetElapsedTime(Unit);
	}

	void Stopwatch::Reset()
	{
		if (Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was running");
		}

		StartTimer = 0.0;
		ElapsedTime = 0.0;
		Started = false;
		Paused = false;
	}

	double Stopwatch::Peek(double Unit)
	{
		if (!Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return 0.0;
		}

		if (Paused)
		{
			return GetElapsedTime(Unit);
		}

		return (Now() - StartTimer) * Unit;
	}

	double Stopwatch::GetStartTime(double Unit) const
	{
		if (StartTimer == 0.0)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return 0.0;
		}

		return StartTimer * Unit;
	}

	double Stopwatch::GetElapsedTime(double Unit) const
	{
		if (ElapsedTime == 0.0)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return 0.0;
		}

		if (Started)
		{
			NEXUS_LOG(Error, Default, "Stopwatch is running");
			return 0.0;
		}

		return ElapsedTime * Unit;
	}

	double Stopwatch::Now() const
	{
		return Platform::GetInstance()->GetProcessorTimer();
	}
}
