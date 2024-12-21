#include "Core/NexusEnginePch.h"
#include "Time.h"

namespace NxEn
{
	//-----------------------------------------------------------------------------------------------------------------------
	// Constant
	//-----------------------------------------------------------------------------------------------------------------------

	static String WeekDaysNames[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	static String MonthsNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	static int32 DayPerMonths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	//-----------------------------------------------------------------------------------------------------------------------
	// C Lib Convertion
	//-----------------------------------------------------------------------------------------------------------------------

	static const uint64 TimeToStringCapacity = 40;
	static char TimeToStringBuffer[TimeToStringCapacity];

	// Make sure Timestamp follow the same structure as tm from C library
	static union TimeInfo
	{
		TimeInfo() {};

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

	//-----------------------------------------------------------------------------------------------------------------------
	// Time
	//-----------------------------------------------------------------------------------------------------------------------

	int64 Time::GetTimeSinceEpoch() const
	{
		int64 Time;
		time(&Time);
		return Time;
	}

	Timestamp Time::Now() const
	{
		int64 Time = GetTimeSinceEpoch();
		localtime_s(&TimeInfo.TM, &Time);
		ConvertFromCLibToNexus();
		return TimeInfo.Stamp;
	}

	String Time::ToString(const Timestamp& Stamp, StringView Format)
	{
		TimeInfo.Stamp = Stamp;
		ConvertFromNexusToCLib(true);
		uint64 Size = strftime(TimeToStringBuffer, TimeToStringCapacity, Format.C(), &TimeInfo.TM);
		return String(TimeToStringBuffer, Size);
	}

	String Time::ToString(const Timespan& Span, StringView Format)
	{
		TimeInfo.Span = Span;
		ConvertFromNexusToCLib(false);
		uint64 Size = strftime(TimeToStringBuffer, TimeToStringCapacity, Format.C(), &TimeInfo.TM);
		return String(TimeToStringBuffer, Size);
	}

	StringView Time::GetWeekDay(int32 Day)
	{
		return WeekDaysNames[Day - 1];
	}

	StringView Time::GetMonth(int32 Month)
	{
		return MonthsNames[Month - 1];
	}

	int32 Time::GetDaysPerMonth(int32 Month, bool Leap)
	{
		NEXUS_ASSERT(Month >= 1 && Month <= 12, "Month has to be between 1-12, value was %d")
		int32 Days = DayPerMonths[Month - 1];
		Days += Leap && Month == 2 ? 1 : 0;
		return Days;
	}

	bool Time::IsLeapYear(int32 Year)
	{
		return (Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0));
	}

	Time::Time()
	{
	}

	Time::~Time()
	{
	}
}
