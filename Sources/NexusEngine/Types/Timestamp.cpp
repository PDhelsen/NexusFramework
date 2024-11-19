#include "Core/NexusEnginePch.h"
#include "Types/Timestamp.h"

#include "Application/Time.h"

namespace NxEn
{
	Timestamp::Timestamp()
		: Seconds(-1), Minutes(-1), Hours(-1), Days(-1), Months(-1), Years(-1), WeekDay(-1), YearDay(-1), SummerTime(false)
	{
	}

	Timestamp::Timestamp(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S, int32 Wd, int32 Yd, bool St)
		: Seconds(S), Minutes(Mn), Hours(H), Days(D), Months(M), Years(Y), WeekDay(Wd), YearDay(Yd), SummerTime(St)
	{
	}

	bool Timestamp::operator==(const Timestamp& Other) const
	{
		return Seconds == Other.Seconds &&
			Minutes == Other.Minutes &&
			Hours == Other.Hours &&
			Days == Other.Days &&
			Months == Other.Months &&
			Years == Other.Years &&
			WeekDay == Other.WeekDay &&
			YearDay == Other.YearDay &&
			SummerTime == Other.SummerTime;
	}

	bool Timestamp::operator!=(const Timestamp& Other) const
	{
		return !(*this == Other);
	}

	bool Timestamp::operator<(const Timestamp& Other) const
	{
		return Years < Other.Years ? true :
			Months < Other.Months ? true :
			Days < Other.Days ? true :
			Hours < Other.Hours ? true :
			Minutes < Other.Minutes ? true :
			Seconds < Other.Seconds;
	}

	bool Timestamp::operator>(const Timestamp& Other) const
	{
		return Years > Other.Years ? true :
			Months > Other.Months ? true :
			Days > Other.Days ? true :
			Hours > Other.Hours ? true :
			Minutes > Other.Minutes ? true :
			Seconds > Other.Seconds;
	}

	bool Timestamp::operator<=(const Timestamp& Other) const
	{
		return !(*this > Other);
	}

	bool Timestamp::operator>=(const Timestamp& Other) const
	{
		return !(*this < Other);
	}

	Timestamp::operator bool() const
	{
		return IsValid();
	}

	StringView Timestamp::GetWeekDay() const
	{
		return Time::GetWeekDay(WeekDay);
	}

	StringView Timestamp::GetMonth() const
	{
		return Time::GetMonth(Months);
	}

	String Timestamp::ToString(StringView Format) const
	{
		return Time::ToString(*this, Format);
	}

	bool Timestamp::IsValid() const
	{
		return Seconds != -1 && Minutes != -1 && Hours != -1 && Days != -1 && Months != -1 && Years != -1;
	}

	bool Timestamp::IsFull() const
	{
		return WeekDay != -1 && YearDay != -1;
	}
}
