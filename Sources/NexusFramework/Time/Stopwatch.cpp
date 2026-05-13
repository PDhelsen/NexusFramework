#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Time/Stopwatch.h"

#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/Time/Time.h"

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

	void Stopwatch::Start()
	{
		if (Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was already started");
			return;
		}

		StartTimer = Time::ProcessorTick();
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

		ElapsedTime += Time::ProcessorTick() - StartTimer;
		Paused = true;

		return Time::Convert(ElapsedTime, Unit);
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

		StartTimer = Globals::PlatformTarget->GetProcessorTimer();
		Paused = false;
	}

	double Stopwatch::Stop(double Unit)
	{
		if (!Started)
		{
			NEXUS_LOG(Warning, Default, "Stopwatch was not started");
			return 0.0;
		}

		ElapsedTime += Paused ? 0 : Time::ProcessorTick() - StartTimer;
		Started = false;
		Paused = false;

		return Time::Convert(ElapsedTime, Unit);
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

	double Stopwatch::Peek(double Unit) const
	{
		if (!Paused)
		{
			return Time::Convert(Time::ProcessorTick() - StartTimer, Unit);
		}

		return Time::Convert(ElapsedTime, Unit);
	}

	double Stopwatch::GetStartTime(double Unit) const
	{
		return Time::Convert(StartTimer, Unit);
	}

	double Stopwatch::GetElapsedTime(double Unit) const
	{
		return Time::Convert(ElapsedTime, Unit);
	}
}
