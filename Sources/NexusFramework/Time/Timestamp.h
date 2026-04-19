#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"
#include "NexusFramework/Time/Timespan.h"

namespace NxFr
{
    struct Timestamp
    {
	public:
		NEXUS_FRAMEWORK_API Timestamp();
		NEXUS_FRAMEWORK_API Timestamp(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S, int32 Wd = -1, int32 Yd = -1, bool St = false);

		NEXUS_FRAMEWORK_API operator bool() const;
		NEXUS_FRAMEWORK_API Timestamp& operator+=(const Timespan& Other);
		NEXUS_FRAMEWORK_API Timestamp& operator-=(const Timespan& Other);

		NEXUS_FRAMEWORK_API StringView GetWeekDay() const;
		NEXUS_FRAMEWORK_API StringView GetMonth() const;
		NEXUS_FRAMEWORK_API int32 GetDaysPerMonth() const;

		NEXUS_FRAMEWORK_API bool IsValid() const;
		NEXUS_FRAMEWORK_API bool IsFull() const;

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

	template<>
	struct StringConverter<Timestamp>
	{
		NEXUS_FRAMEWORK_API static StringView GetFormat(bool Pretty);
		NEXUS_FRAMEWORK_API static void ToString(const Timestamp& Data, String& Result, StringView Format);
		NEXUS_FRAMEWORK_API static void FromString(StringView Data, Timestamp& Result, StringView Format);
	};

	NEXUS_FRAMEWORK_API bool operator==(const Timestamp& A, const Timestamp& B);
	NEXUS_FRAMEWORK_API bool operator!=(const Timestamp& A, const Timestamp& B);
	NEXUS_FRAMEWORK_API bool operator<(const Timestamp& A, const Timestamp& B);
	NEXUS_FRAMEWORK_API bool operator>(const Timestamp& A, const Timestamp& B);
	NEXUS_FRAMEWORK_API bool operator<=(const Timestamp& A, const Timestamp& B);
	NEXUS_FRAMEWORK_API bool operator>=(const Timestamp& A, const Timestamp& B);
	NEXUS_FRAMEWORK_API Timestamp operator+(const Timestamp& A, const Timespan& B);
	NEXUS_FRAMEWORK_API Timestamp operator-(const Timestamp& A, const Timespan& B);
}
