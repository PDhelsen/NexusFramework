#include "Core/NexusEnginePch.h"
#include "Stopwatch.h"

#include "Platform/Platform.h"

namespace NxEn
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

		StartTimer = Platform::GetInstance()->GetProcessorTimer();
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

		ComputeElapsed();
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

		ComputeElapsed();
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

	void Stopwatch::ComputeElapsed()
	{
		double EndTimer = Platform::GetInstance()->GetProcessorTimer();
		ElapsedTime += EndTimer - StartTimer;
	}
}
