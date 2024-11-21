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

	private:
		Time();
		~Time();
	};
}

