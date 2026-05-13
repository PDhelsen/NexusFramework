#pragma once

#include "NexusFramework/Misc/Color/Color.h"

#include "NexusFramework/Types/Strings/Utility.h"

namespace NxFr
{
	namespace StringUtility
	{
		template<>
		struct Converter<Color>
		{
			static StringView GetFormat(bool Pretty)
			{
				return Pretty ? "(%.2f, %.2f, %.2f, %.2f)" : "(%f, %f, %f, %f)";
			}

			static void ToString(const Color& Data, String& Result, StringView Format = "")
			{
				StringUtility::Format(Result, StringUtility::ConvertionFormat<Color>(Format, true), Data.r, Data.g, Data.b, Data.a);
			}

			static void FromString(StringView Data, Color& Result, StringView Format = "")
			{
				StringUtility::Scan(Data, StringUtility::ConvertionFormat<Color>(Format, false), &Result.r, &Result.g, &Result.b, &Result.a);
			}
		};
	}
}
