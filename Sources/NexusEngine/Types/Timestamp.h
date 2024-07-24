#pragma once

#include "Types/Integer.h"

namespace NxEn
{
    struct Timestamp
    {
        int32 Seconds;
        int32 Minutes;
        int32 Hours;
        int32 Days;
        int32 Months;
        int32 Year;
        int32 WeekDay;
        int32 YearDay;
        int32 SummerTime;
    };
}
