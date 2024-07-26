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
		int64 Time = GetTimeSinceEpoch();
		
		// TODO: Implementation - Time - Convert time since epoch to Timestamp
		std::tm TimeInfo;
		localtime_s(&TimeInfo, &Time);

		Timestamp Stamp = 
		{ 
			.Seconds = TimeInfo.tm_sec,
			.Minutes = TimeInfo.tm_min,
			.Hours = TimeInfo.tm_hour,
			.Days = TimeInfo.tm_mday,
			.Months = TimeInfo.tm_mon,
			.Year = TimeInfo.tm_year,
			.WeekDay = TimeInfo.tm_wday,
			.YearDay = TimeInfo.tm_yday,
			.SummerTime = TimeInfo.tm_isdst,
		};
		return Stamp;
	}

	Time::Time()
	{
	}

	Time::~Time()
	{
	}
}
