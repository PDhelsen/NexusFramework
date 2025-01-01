#pragma once

#include "Core/NexusEngineCore.h"
#include "Types/Numbers/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Time/Timespan.h"

namespace NxEn
{
    struct Timestamp
    {
	public:
		inline static const String Format = "%A - %B %d %Y - %H:%M:%S";

		NEXUS_ENGINE_API Timestamp();
		NEXUS_ENGINE_API Timestamp(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S, int32 Wd = -1, int32 Yd = -1, bool St = false);

		NEXUS_ENGINE_API Timestamp& operator+=(const Timespan& Other);
		NEXUS_ENGINE_API Timestamp& operator-=(const Timespan& Other);
		NEXUS_ENGINE_API explicit operator bool() const;

		NEXUS_ENGINE_API StringView GetWeekDay() const;
		NEXUS_ENGINE_API StringView GetMonth() const;
		NEXUS_ENGINE_API int32 GetDaysPerMonth() const;

		NEXUS_ENGINE_API String ToString(StringView Format = Format) const;

		NEXUS_ENGINE_API bool IsValid() const;
		NEXUS_ENGINE_API bool IsFull() const;

        int32 Seconds;
        int32 Minutes;
        int32 Hours;
        int32 Days;
        int32 Months;
        int32 Years;
        int32 WeekDay;
        int32 YearDay;
        bool DayLightSaving;
    };

	NEXUS_ENGINE_API bool operator==(const Timestamp& A, const Timestamp& B);
	NEXUS_ENGINE_API bool operator!=(const Timestamp& A, const Timestamp& B);
	NEXUS_ENGINE_API bool operator<(const Timestamp& A, const Timestamp& B);
	NEXUS_ENGINE_API bool operator>(const Timestamp& A, const Timestamp& B);
	NEXUS_ENGINE_API bool operator<=(const Timestamp& A, const Timestamp& B);
	NEXUS_ENGINE_API bool operator>=(const Timestamp& A, const Timestamp& B);
	NEXUS_ENGINE_API Timestamp operator+(const Timestamp& A, const Timespan& B);
	NEXUS_ENGINE_API Timestamp operator-(const Timestamp& A, const Timespan& B);
}
