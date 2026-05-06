#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Time/Timespan.h"
#include "NexusFramework/Time/Timestamp.h"

namespace NxFr
{
	namespace Time
	{
		inline constexpr double SecondToMicro = 1000000.0;
		inline constexpr double SecondToMilli = 1000.0;
		inline constexpr double SecondToMin = 1.0 / 60.0;
		inline constexpr double SecondToHrs = (1.0 / 60.0) * (1.0 / 60.0);
		inline constexpr double SecondToDay = (1.0 / 60.0) * (1.0 / 60.0) * (1.0 / 24);

		inline constexpr double MicroToSecond = 0.000001;
		inline constexpr double MilliToSecond = 0.001;
		inline constexpr double MinToSecond = 60.0;
		inline constexpr double HrsToSecond = 60.0 * 60;
		inline constexpr double DayToSecond = 60.0 * 60 * 24;

		NEXUS_FRAMEWORK_API Timestamp Now();
		NEXUS_FRAMEWORK_API int64 TimeSinceEpoch();
		NEXUS_FRAMEWORK_API uint64 ProcessorTick();
		NEXUS_FRAMEWORK_API double Convert(double Value, double Unit = 1.0);

		NEXUS_FRAMEWORK_API StringView GetWeekDay(int32 Day);
		NEXUS_FRAMEWORK_API StringView GetMonth(int32 Month);
		NEXUS_FRAMEWORK_API int32 GetDaysPerMonth(int32 Month, bool Leap);
		NEXUS_FRAMEWORK_API bool IsLeapYear(int32 Year);
	};
}

