#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Types/Numbers/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Time/Timespan.h"
#include "Time/Timestamp.h"

namespace NxFr
{
	namespace Time
	{
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

		NEXUS_FRAMEWORK_API int64 GetTimeSinceEpoch();
		NEXUS_FRAMEWORK_API Timestamp Now();

		NEXUS_FRAMEWORK_API StringView GetWeekDay(int32 Day);
		NEXUS_FRAMEWORK_API StringView GetMonth(int32 Month);
		NEXUS_FRAMEWORK_API int32 GetDaysPerMonth(int32 Month, bool Leap);
		NEXUS_FRAMEWORK_API bool IsLeapYear(int32 Year);

		NEXUS_FRAMEWORK_API String ToString(const Timestamp& Stamp, StringView Format = Timestamp::Format);
		NEXUS_FRAMEWORK_API String ToString(const Timespan& Span, StringView Format = Timespan::Format);
	};
}

