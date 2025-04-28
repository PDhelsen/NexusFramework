#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Time/Time.h"

#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr
{
	namespace Time
	{

#pragma region Constant

		static String WeekDaysNames[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
		static String MonthsNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		static int32 DayPerMonths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		static Buffer& GetLocalBuffer() { static Buffer LocalBuffer(64, nullptr); return LocalBuffer; }

#pragma endregion

#pragma region C Lib Convertion

		// Make sure Timestamp follow the same structure as tm from C library
		static union TimeInfo
		{
			TimeInfo()
				: Stamp()
			{
			};

			tm TM;
			Timestamp Stamp;
			Timespan Span;
		} TimeInfo;

		static void ConvertFromNexusToCLib(bool Date)
		{
			TimeInfo.TM.tm_year -= 1900;
			TimeInfo.TM.tm_mon -= 1;
			if (Date)
			{
				TimeInfo.TM.tm_wday = TimeInfo.Stamp.WeekDay == 7 ? 0 : TimeInfo.Stamp.WeekDay;
				TimeInfo.TM.tm_isdst = TimeInfo.Stamp.DayLightSaving ? 1 : 0;
			}
			else
			{
				TimeInfo.TM.tm_wday = 0;
				TimeInfo.TM.tm_yday = 0;
				TimeInfo.TM.tm_isdst = 0;
			}
		}

		static void ConvertFromCLibToNexus()
		{
			TimeInfo.Stamp.Years += 1900;
			TimeInfo.Stamp.Months += 1;
			TimeInfo.Stamp.WeekDay = TimeInfo.Stamp.WeekDay == 0 ? 7 : TimeInfo.Stamp.WeekDay;
			TimeInfo.Stamp.DayLightSaving = TimeInfo.TM.tm_isdst != 0 ? true : false;
		}

#pragma endregion

#pragma region Time

		int64 GetTimeSinceEpoch()
		{
			int64 Time;
			time(&Time);
			return Time;
		}

		Timestamp Now()
		{
			int64 Time = GetTimeSinceEpoch();
			localtime_s(&TimeInfo.TM, &Time);
			ConvertFromCLibToNexus();
			return TimeInfo.Stamp;
		}

		StringView GetWeekDay(int32 Day)
		{
			NEXUS_ASSERT(Day >= 1 && Day <= 7, Default, "Day has to be between 1-7, value was %d");

			return WeekDaysNames[Day - 1];
		}

		StringView GetMonth(int32 Month)
		{
			NEXUS_ASSERT(Month >= 1 && Month <= 12, Default, "Month has to be between 1-12, value was %d");

			return MonthsNames[Month - 1];
		}

		int32 GetDaysPerMonth(int32 Month, bool Leap)
		{
			NEXUS_ASSERT(Month >= 1 && Month <= 12, Default, "Month has to be between 1-12, value was %d");

			int32 Days = DayPerMonths[Month - 1];
			Days += Leap && Month == 2 ? 1 : 0;
			return Days;
		}

		bool IsLeapYear(int32 Year)
		{
			return (Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0));
		}

		String ToString(const Timestamp& Stamp, StringView Format)
		{
			Buffer& LocalBuffer = GetLocalBuffer();

			TimeInfo.Stamp = Stamp;
			ConvertFromNexusToCLib(true);
			uint64 Size = strftime(LocalBuffer.GetPtr<char>(), LocalBuffer.GetCount(), Format.C(), &TimeInfo.TM);
			return String(LocalBuffer.GetPtr<char>(), Size);
		}

		String ToString(const Timespan& Span, StringView Format)
		{
			Buffer& LocalBuffer = GetLocalBuffer();

			TimeInfo.Span = Span;
			ConvertFromNexusToCLib(false);
			uint64 Size = strftime(LocalBuffer.GetPtr<char>(), LocalBuffer.GetCount(), Format.C(), &TimeInfo.TM);
			return String(LocalBuffer.GetPtr<char>(), Size);
		}

#pragma endregion

	}
}
