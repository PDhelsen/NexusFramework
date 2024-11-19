#include "Core/NexusEnginePch.h"
#include "Time.h"

namespace NxEn
{
	const static uint64 TimeToStringCapacity = 40;
	static char TimeToStringBuffer[TimeToStringCapacity];
	static String WeekDaysNames[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	static String MonthsNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	// Make sure Timestamp follow the same structure as tm from C library
	static union TimeInfo
	{
		TimeInfo() {};

		tm TM;
		Timestamp Stamp;
	} TimeInfo;

	static void ConvertFromStampToTm()
	{
		TimeInfo.TM.tm_mon -= 1;
		TimeInfo.TM.tm_year -= 1900;
		TimeInfo.TM.tm_wday = TimeInfo.Stamp.WeekDay == 7 ? 0 : TimeInfo.Stamp.WeekDay;
		TimeInfo.TM.tm_isdst = TimeInfo.Stamp.SummerTime ? 1 : 0;
	}

	static void ConvertFromTmToStamp()
	{
		TimeInfo.Stamp.Months += 1;
		TimeInfo.Stamp.Years += 1900;
		TimeInfo.Stamp.WeekDay = TimeInfo.Stamp.WeekDay == 0 ? 7 : TimeInfo.Stamp.WeekDay;
		TimeInfo.Stamp.SummerTime = TimeInfo.TM.tm_isdst != 0 ? true : false;
	}

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
		ConvertFromTmToStamp();
		return TimeInfo.Stamp;
	}

	StringView Time::GetWeekDay(int32 Day)
	{
		return WeekDaysNames[Day - 1];
	}

	StringView Time::GetMonth(int32 Month)
	{
		return MonthsNames[Month - 1];
	}

	String Time::ToString(const Timestamp& Stamp, StringView Format)
	{
		TimeInfo.Stamp = Stamp;
		ConvertFromStampToTm();
		uint64 Size = strftime(TimeToStringBuffer, TimeToStringCapacity, Format.C(), &TimeInfo.TM);
		return String(TimeToStringBuffer, Size);
	}

	Time::Time()
	{
	}

	Time::~Time()
	{
	}
}
