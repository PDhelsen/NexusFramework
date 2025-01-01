#include "Core/NexusEnginePch.h"
#include "Timestamp.h"

#include "Time/Time.h"

namespace NxEn
{
	Timestamp::Timestamp()
		: Seconds(-1), Minutes(-1), Hours(-1), Days(-1), Months(-1), Years(-1), WeekDay(-1), YearDay(-1), DayLightSaving(false)
	{
	}

	Timestamp::Timestamp(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S, int32 Wd, int32 Yd, bool St)
		: Seconds(S), Minutes(Mn), Hours(H), Days(D), Months(M), Years(Y), WeekDay(Wd), YearDay(Yd), DayLightSaving(St)
	{
		NEXUS_ASSERT(Months > 0 && Months <= 12, Default, "Invalid Months");
		NEXUS_ASSERT(Days > 0 && Days <= GetDaysPerMonth(), Default, "Invalid Days");
		NEXUS_ASSERT(Hours >= 0 && Hours < 24, Default, "Invalid Hours");
		NEXUS_ASSERT(Minutes >= 0 && Minutes < 60, Default, "Invalid Minutes");
		NEXUS_ASSERT(Seconds >= 0 && Seconds < 60, Default, "Invalid Seconds");
	}

	Timestamp& Timestamp::operator+=(const Timespan& Other)
	{
		Seconds += Other.Seconds;
		if (Seconds >= 60)
		{
			Minutes += Seconds / 60;
			Seconds %= 60;
		}

		Minutes += Other.Minutes;
		if (Minutes >= 60)
		{
			Hours += Minutes / 60;
			Minutes %= 60;
		}

		Hours += Other.Hours;
		if (Hours >= 24)
		{
			Days += Hours / 24;
			Hours %= 24;
		}

		int32 DaysToAdd = Other.Days;
		while (true)
		{
			int32 DaysPerMonth = GetDaysPerMonth();
			int32 DayDelta = DaysPerMonth - Days;
			if (DaysToAdd > DayDelta)
			{
				DaysToAdd -= (DayDelta + 1);

				Months++;
				if (Months > 12)
				{
					Years++;
					Months = 1;
				}

				Days = 1;
			}
			else
			{
				Days += DaysToAdd;
				break;
			}
		}

		Months += Other.Months;
		if (Months >= 12)
		{
			Years += Months / 12;
			Months %= 12;
		}

		Years += Other.Years;

		WeekDay = -1;
		YearDay = -1;

		return *this;
	}

	Timestamp& Timestamp::operator-=(const Timespan& Other)
	{
		Seconds -= Other.Seconds;
		if (Seconds < 0)
		{
			Minutes -= (Math::Abs(Seconds) + 59) / 60;
			Seconds = (Seconds % 60 + 60) % 60;
		}

		Minutes -= Other.Minutes;
		if (Minutes < 0)
		{
			Hours -= (Math::Abs(Minutes) + 59) / 60;
			Minutes = (Minutes % 60 + 60) % 60;
		}

		Hours -= Other.Hours;
		if (Hours < 0)
		{
			Days -= (Math::Abs(Hours) + 23) / 24;
			Hours = (Hours % 24 + 24) % 24;
		}

		int32 DaysToSub = Other.Days;
		while (true)
		{
			if (DaysToSub > Days)
			{
				DaysToSub -= Days;

				Months--;
				if (Months <= 0)
				{
					Years--;
					Months = 12;
				}

				Days = GetDaysPerMonth();
			}
			else
			{
				Days -= DaysToSub;
				break;
			}
		}

		Months -= Other.Months;
		if (Months < 0)
		{
			Years -= (Math::Abs(Months) + 11) / 12;
			Months = (Months % 12 + 12) % 12;
		}

		Years -= Other.Years;

		WeekDay = -1;
		YearDay = -1;

		return *this;
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

	int32 Timestamp::GetDaysPerMonth() const
	{
		return Time::GetDaysPerMonth(Months, Time::IsLeapYear(Years));
	}

	String Timestamp::ToString(StringView Format) const
	{
		return Time::ToString(*this, Format);
	}
	
	bool Timestamp::IsValid() const
	{
		return Seconds >= 0 && Seconds < 60 && Minutes >= 0 && Minutes < 60 && Hours >= 0 && Hours < 24 && Days > 0 && Days <= GetDaysPerMonth() && Months > 0 && Months <= 12;
	}

	bool Timestamp::IsFull() const
	{
		return WeekDay != -1 && YearDay != -1;
	}

	bool operator==(const Timestamp& A, const Timestamp& B)
	{
		return A.Seconds == B.Seconds &&
			A.Minutes == B.Minutes &&
			A.Hours == B.Hours &&
			A.Days == B.Days &&
			A.Months == B.Months &&
			A.Years == B.Years &&
			A.WeekDay == B.WeekDay &&
			A.YearDay == B.YearDay &&
			A.DayLightSaving == B.DayLightSaving;
	}

	bool operator!=(const Timestamp& A, const Timestamp& B)
	{
		return !(A == B);
	}

	bool operator<(const Timestamp& A, const Timestamp& B)
	{
		return A.Years < B.Years ? true :
			A.Months < B.Months ? true :
			A.Days < B.Days ? true :
			A.Hours < B.Hours ? true :
			A.Minutes < B.Minutes ? true :
			A.Seconds < B.Seconds;
	}

	bool operator>(const Timestamp& A, const Timestamp& B)
	{
		return A.Years > B.Years ? true :
			A.Months > B.Months ? true :
			A.Days > B.Days ? true :
			A.Hours > B.Hours ? true :
			A.Minutes > B.Minutes ? true :
			A.Seconds > B.Seconds;
	}

	bool operator<=(const Timestamp& A, const Timestamp& B)
	{
		return !(A > B);
	}

	bool operator>=(const Timestamp& A, const Timestamp& B)
	{
		return !(A < B);
	}

	Timestamp operator+(const Timestamp& A, const Timespan& B)
	{
		Timestamp Result = A;
		Result += B;
		return Result;
	}

	Timestamp operator-(const Timestamp& A, const Timespan& B)
	{
		Timestamp Result = A;
		Result -= B;
		return Result;
	}
}
