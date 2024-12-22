#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Time
	{
		NEXUS_ENGINE_API int64 GetTimeSinceEpoch();
		NEXUS_ENGINE_API Timestamp Now();

		NEXUS_ENGINE_API String ToString(const Timestamp& Stamp, StringView Format = Timestamp::Format);
		NEXUS_ENGINE_API String ToString(const Timespan& Span, StringView Format = Timespan::Format);
		NEXUS_ENGINE_API StringView GetWeekDay(int32 Day);
		NEXUS_ENGINE_API StringView GetMonth(int32 Month);
		NEXUS_ENGINE_API int32 GetDaysPerMonth(int32 Month, bool Leap);
		NEXUS_ENGINE_API bool IsLeapYear(int32 Year);

		constexpr double SecondToMicro = 1000000.0;
		constexpr double SecondToMilli = 1000.0;
		constexpr double SecondToMin = 1.0 / 60.0;
		constexpr double SecondToHrs = (1.0 / 60.0) * (1.0 / 60.0);
		constexpr double SecondToDay = (1.0 / 60.0) * (1.0 / 60.0) * (1.0 / 24);

		constexpr double MicroToSecond = 0.000001;
		constexpr double MilliToSecond = 0.001;
		constexpr double MinToSecond = 60.0;
		constexpr double HrsToSecond = 60.0 * 60;
		constexpr double DayToSecond = 60.0 * 60 * 24;
	};
}

