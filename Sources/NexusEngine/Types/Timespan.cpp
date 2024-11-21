#include "Core/NexusEnginePch.h"
#include "Types/Timespan.h"

#include "Application/Time.h"

namespace NxEn
{
	Timespan::Timespan()
		: Seconds(0), Minutes(0), Hours(0), Days(0), Months(0), Years(0)
	{
	}

	Timespan::Timespan(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S)
		: Seconds(S), Minutes(Mn), Hours(H), Days(D), Months(M), Years(Y)
	{

	}

	Timespan& Timespan::operator+=(const Timespan& Other)
	{
		Seconds += Other.Seconds;
		Minutes += Other.Minutes;
		Hours += Other.Hours;
		Days += Other.Days;
		Months += Other.Months;
		Years += Other.Years;

		return *this;
	}

	Timespan& Timespan::operator-=(const Timespan& Other)
	{
		Seconds -= Other.Seconds;
		Minutes -= Other.Minutes;
		Hours -= Other.Hours;
		Days -= Other.Days;
		Months -= Other.Months;
		Years -= Other.Years;

		return *this;
	}

	void Timespan::Normalize()
	{
		if (Seconds >= 60)
		{
			Minutes += Seconds / 60;
			Seconds %= 60;
		}
		else if (Seconds < 0)
		{
			Minutes -= (Math::Abs(Seconds) + 59) / 60;
			Seconds = (Seconds % 60 + 60) % 60;
		}

		if (Minutes >= 60)
		{
			Hours += Minutes / 60;
			Minutes %= 60;
		}
		else if (Minutes < 0)
		{
			Hours -= (Math::Abs(Minutes) + 59) / 60;
			Minutes = (Minutes % 60 + 60) % 60;
		}

		if (Hours >= 24)
		{
			Days += Hours / 24;
			Hours %= 24;
		}
		else if (Hours < 0)
		{
			Days -= (Math::Abs(Hours) + 23) / 24;
			Hours = (Hours % 24 + 24) % 24;
		}

		if (Days >= 31)
		{
			Months += Days / 31;
			Days %= 31;
		}
		else if (Days < 0)
		{
			Months -= (Math::Abs(Days) + 30) / 31;
			Days = (Days % 31 + 31) % 31;
		}

		if (Months >= 12)
		{
			Years += Months / 12;
			Months %= 12;
		}
		else if (Months < 0)
		{
			Years -= (Math::Abs(Months) + 11) / 12;
			Months = (Months % 12 + 12) % 12;
		}
	}

	String Timespan::ToString(StringView Format) const
	{
		return Time::ToString(*this, Format);
	}

	bool Timespan::IsValid() const
	{
		return Seconds >= 0 && Minutes >= 0 && Hours >= 0 && Days >= 0 && Months >= 0 && Years >= 0;
	}

	bool operator==(const Timespan& A, const Timespan& B)
	{
		return A.Seconds == B.Seconds &&
			A.Minutes == B.Minutes &&
			A.Hours == B.Hours &&
			A.Days == B.Days &&
			A.Months == B.Months &&
			A.Years == B.Years;
	}

	bool operator!=(const Timespan& A, const Timespan& B)
	{
		return !(A == B);
	}

	bool operator<(const Timespan& A, const Timespan& B)
	{
		return A.Years < B.Years ? true :
			A.Months < B.Months ? true :
			A.Days < B.Days ? true :
			A.Hours < B.Hours ? true :
			A.Minutes < B.Minutes ? true :
			A.Seconds < B.Seconds;
	}

	bool operator>(const Timespan& A, const Timespan& B)
	{
		return A.Years > B.Years ? true :
			A.Months > B.Months ? true :
			A.Days > B.Days ? true :
			A.Hours > B.Hours ? true :
			A.Minutes > B.Minutes ? true :
			A.Seconds > B.Seconds;
	}

	bool operator<=(const Timespan& A, const Timespan& B)
	{
		return !(A > B);
	}

	bool operator>=(const Timespan& A, const Timespan& B)
	{
		return !(A < B);
	}

	Timespan operator+(const Timespan& A, const Timespan& B)
	{
		Timespan Result = A;
		Result += B;
		return Result;
	}

	Timespan operator-(const Timespan& A, const Timespan& B)
	{
		Timespan Result = A;
		Result -= B;
		return Result;
	}
}
