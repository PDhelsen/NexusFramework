#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	class Time
	{
	public:
		NEXUS_ENGINE_API int64 GetTimeSinceEpoch() const;
		NEXUS_ENGINE_API Timestamp Now() const;

		NEXUS_ENGINE_API static String ToString(const Timestamp& Stamp, StringView Format = Timestamp::Format);
		NEXUS_ENGINE_API static String ToString(const Timespan& Span, StringView Format = Timespan::Format);
		NEXUS_ENGINE_API static StringView GetWeekDay(int32 Day);
		NEXUS_ENGINE_API static StringView GetMonth(int32 Month);
		NEXUS_ENGINE_API static int32 GetDaysPerMonth(int32 Month, bool Leap);
		NEXUS_ENGINE_API static bool IsLeapYear(int32 Year);

		NEXUS_ENGINE_API static Time* GetInstance() { static Time* Instance = new Time(); return Instance; }

		inline static const double SecondToMicro = 1000000.0;
		inline static const double SecondToMilli = 1000.0;
		inline static const double SecondToMin = 1.0 / 60.0;
		inline static const double SecondToHrs = (1.0 / 60.0) * (1.0 / 60.0);
		inline static const double SecondToDay = (1.0 / 60.0) * (1.0 / 60.0) * (1.0 / 24);

		inline static const double MicroToSecond = 0.000001;
		inline static const double MilliToSecond = 0.001;
		inline static const double MinToSecond = 60.0;
		inline static const double HrsToSecond = 60.0 * 60;
		inline static const double DayToSecond = 60.0 * 60 * 24;

	private:
		Time();
		~Time();
	};
}

