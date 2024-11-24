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
			NEXUS_LOG(Engine, Warning, NxEn::LoggerChannel::Default, "Stopwatch was already started");
			return;
		}

		StartTimer = Platform::GetInstance()->GetProcessorTimer();
		ElapsedTime = 0.0;
		Started = true;
		Paused = false;
	}

	double Stopwatch::Pause(double Unit)
	{
		if (Paused || !Started)
		{
			NEXUS_LOG(Engine, Warning, NxEn::LoggerChannel::Default, "Stopwatch was already paused");
			return 0.0;
		}

		ComputeElapsed();
		Paused = true;

		return GetElapsedTime(Unit);
	}

	void Stopwatch::Resume()
	{
		if (!Paused || !Started)
		{
			NEXUS_LOG(Engine, Warning, NxEn::LoggerChannel::Default, "Stopwatch was not paused");
			return;
		}

		StartTimer = Platform::GetInstance()->GetProcessorTimer();
		Paused = false;
	}

	double Stopwatch::Stop(double Unit)
	{
		if (!Started || Paused)
		{
			NEXUS_LOG(Engine, Warning, NxEn::LoggerChannel::Default, "Stopwatch was not started");
			return 0.0;
		}

		ComputeElapsed();
		Started = false;
		Paused = false;

		return GetElapsedTime(Unit);
	}

	void Stopwatch::Reset()
	{
		StartTimer = 0.0;
		ElapsedTime = 0.0;
		Started = false;
		Paused = false;
	}

	double Stopwatch::GetElapsedTime(double Unit)
	{
		return ElapsedTime * Unit;
	}

	void Stopwatch::ComputeElapsed()
	{
		double EndTimer = Platform::GetInstance()->GetProcessorTimer();
		ElapsedTime += EndTimer - StartTimer;
	}
}
