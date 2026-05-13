#pragma once

#include "NexusFramework/Time/Timespan.h"
#include "NexusFramework/Time/Timestamp.h"
#include "NexusFramework/Time/Stopwatch.h"

#include "NexusFramework/Types/Strings/Utility.h"

#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	namespace StringUtility
	{
		// In Time.cpp due to C-Lib conversion
		template<>
		struct NEXUS_FRAMEWORK_API Converter<Timestamp>
		{
			static StringView GetFormat(bool Pretty);
			static void ToString(const Timestamp& Data, String& Result, StringView Format);
			static void FromString(StringView Data, Timestamp& Result, StringView Format);
		};

		// In Time.cpp due to C-Lib conversion
		template<>
		struct NEXUS_FRAMEWORK_API Converter<Timespan>
		{
			static StringView GetFormat(bool Pretty);
			static void ToString(const Timespan& Data, String& Result, StringView Format = "");
			static void FromString(StringView Data, Timespan& Result, StringView Format = "");
		};

		template<>
		struct Converter<Stopwatch>
		{
			static StringView GetFormat(bool Pretty)
			{
				return Pretty ? "%.2f" : "%f";
			}

			static void ToString(const Stopwatch& Data, String& Result, StringView Format = "")
			{
				StringUtility::Format(Result, StringUtility::ConvertionFormat<Stopwatch>(Format, true), Data.Peek());
			}

			static void FromString(StringView Data, Stopwatch& Result, StringView Format = "")
			{
				NEXUS_ASSERT(false, Default, "Unsupported FromString with Stopwatch");
			}
		};
	}
}
