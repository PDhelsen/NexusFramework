#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Time/Time.h"

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/Time/Misc/String.h"

namespace NxFr
{
#pragma region Constant

	static Buffer& GetLocalBuffer() { static thread_local Buffer LocalBuffer(64, nullptr); return LocalBuffer; }

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

	namespace Time
	{
		Timestamp Now()
		{
			int64 Time = TimeSinceEpoch();
			localtime_s(&TimeInfo.TM, &Time);
			ConvertFromCLibToNexus();
			return TimeInfo.Stamp;
		}

		int64 TimeSinceEpoch()
		{
			int64 Time;
			time(&Time);
			return Time;
		}

		double ProcessorTick()
		{
			return Globals::PlatformTarget->GetProcessorTimer();
		}

		double Convert(double Value, double Unit)
		{
			return Value * Unit;
		}

		StringView GetWeekDay(int32 Day)
		{
			NX_ASSERT(Day >= 1 && Day <= 7, Default, "Day has to be between 1-7, value was %d");

			return WeekDaysNames[Day - 1];
		}

		StringView GetMonth(int32 Month)
		{
			NX_ASSERT(Month >= 1 && Month <= 12, Default, "Month has to be between 1-12, value was %d");

			return MonthsNames[Month - 1];
		}

		int32 GetDaysPerMonth(int32 Month, bool Leap)
		{
			NX_ASSERT(Month >= 1 && Month <= 12, Default, "Month has to be between 1-12, value was %d");

			int32 Days = DayPerMonths[Month - 1];
			Days += Leap && Month == 2 ? 1 : 0;
			return Days;
		}

		bool IsLeapYear(int32 Year)
		{
			return (Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0));
		}
	}

#pragma endregion

#pragma region StringUtility - Converter

	namespace StringUtility
	{
		StringView Converter<Timestamp>::GetFormat(bool Pretty)
		{
			return "%A - %B %d %Y - %H:%M:%S";
		}

		void Converter<Timestamp>::ToString(const Timestamp& Data, String& Result, StringView Format)
		{
			Buffer& LocalBuffer = GetLocalBuffer();
			Format = StringUtility::ConvertionFormat<Timestamp>(Format);

			TimeInfo.Stamp = Data;
			ConvertFromNexusToCLib(true);
			uint64 Size = strftime(LocalBuffer.GetPtr<char>(), LocalBuffer.GetCount(), Format.C(), &TimeInfo.TM);
			Result = String(LocalBuffer.GetPtr<char>(), Size);
		}

		void Converter<Timestamp>::FromString(StringView Data, Timestamp& Result, StringView Format)
		{
			Format = StringUtility::ConvertionFormat<Timestamp>(Format);
			StringUtility::Scan(Data, Format, &TimeInfo.TM);
			Result = TimeInfo.Stamp;
		}

		StringView Converter<Timespan>::GetFormat(bool Pretty)
		{
			return "%Yy%mm%dd - %Hh%Mm%Ss";
		}

		void Converter<Timespan>::ToString(const Timespan& Data, String& Result, StringView Format)
		{
			Buffer& LocalBuffer = GetLocalBuffer();
			Format = StringUtility::ConvertionFormat<Timespan>(Format);

			TimeInfo.Span = Data;
			ConvertFromNexusToCLib(false);
			uint64 Size = strftime(LocalBuffer.GetPtr<char>(), LocalBuffer.GetCount(), Format.C(), &TimeInfo.TM);
			Result = String(LocalBuffer.GetPtr<char>(), Size);
		}

		void Converter<Timespan>::FromString(StringView Data, Timespan& Result, StringView Format)
		{
			Format = StringUtility::ConvertionFormat<Timespan>(Format);
			StringUtility::Scan(Data, Format, &TimeInfo.TM);
			Result = TimeInfo.Span;
		}
	}
#pragma endregion
}
