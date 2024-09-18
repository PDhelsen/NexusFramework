#include "Core/NexusEnginePch.h"
#include "Time.h"

namespace NxEn
{
	Time* Time::Instance = new Time();

	int64 Time::GetTimeSinceEpoch() const
	{
		int64 Time;
		time(&Time);
		return Time;
	}

	Timestamp Time::Now() const
	{
		// Make sure Timestamp follow the same structure as tm from C library
		union
		{
			tm TM;
			Timestamp Stamp;
		} TimeInfo;

		int64 Time = GetTimeSinceEpoch();
		localtime_s(&TimeInfo.TM, &Time);
		return TimeInfo.Stamp;
	}

	Time::Time()
	{
	}

	Time::~Time()
	{
	}
}
