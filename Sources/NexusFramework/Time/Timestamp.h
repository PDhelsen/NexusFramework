#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Time/Timespan.h"

namespace NxFr
{
    struct NX_FRAMEWORK_API Timestamp
    {
		Timestamp();
		Timestamp(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S, int32 Wd = -1, int32 Yd = -1, bool St = false);

		operator bool() const;
		Timestamp& operator+=(const Timespan& Other);
		Timestamp& operator-=(const Timespan& Other);

		StringView GetWeekDay() const;
		StringView GetMonth() const;
		int32 GetDaysPerMonth() const;

		bool IsValid() const;
		bool IsFull() const;

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

	NX_FRAMEWORK_API bool operator==(const Timestamp& A, const Timestamp& B);
	NX_FRAMEWORK_API bool operator!=(const Timestamp& A, const Timestamp& B);
	NX_FRAMEWORK_API bool operator<(const Timestamp& A, const Timestamp& B);
	NX_FRAMEWORK_API bool operator>(const Timestamp& A, const Timestamp& B);
	NX_FRAMEWORK_API bool operator<=(const Timestamp& A, const Timestamp& B);
	NX_FRAMEWORK_API bool operator>=(const Timestamp& A, const Timestamp& B);
	NX_FRAMEWORK_API Timestamp operator+(const Timestamp& A, const Timespan& B);
	NX_FRAMEWORK_API Timestamp operator-(const Timestamp& A, const Timespan& B);
}
