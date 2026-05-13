#pragma once

#include "NexusFramework/Misc/Hash/Hash.h"

#include "NexusFramework/Time/Timespan.h"
#include "NexusFramework/Time/Timestamp.h"
#include "NexusFramework/Time/Stopwatch.h"

namespace NxFr
{
	namespace HashUtility
	{
		template<typename H>
		struct Hasher<Timespan, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Timespan& Data)
			{
				State.Accumulate(Data.Seconds);
				State.Accumulate(Data.Minutes);
				State.Accumulate(Data.Hours);
				State.Accumulate(Data.Days);
				State.Accumulate(Data.Months);
				State.Accumulate(Data.Years);
			}
		};

		template<typename H>
		struct Hasher<Timestamp, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Timestamp& Data)
			{
				State.Accumulate(Data.Seconds);
				State.Accumulate(Data.Minutes);
				State.Accumulate(Data.Hours);
				State.Accumulate(Data.Days);
				State.Accumulate(Data.Months);
				State.Accumulate(Data.Years);
				State.Accumulate(Data.WeekDay);
				State.Accumulate(Data.YearDay);
				State.Accumulate(Data.DayLightSaving);
			}
		};

		template<typename H>
		struct Hasher<Stopwatch, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Stopwatch& Data)
			{
				State.Accumulate(Data.Peek());
			}
		};
	}
}
