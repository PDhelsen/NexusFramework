#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"

namespace NxFr
{
	struct NEXUS_FRAMEWORK_API Timespan
	{
	public:
		Timespan();
		Timespan(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S);

		operator bool() const;
		Timespan& operator+=(const Timespan& Other);
		Timespan& operator-=(const Timespan& Other);

		void Normalize();

		bool IsValid() const;

		int32 Seconds;
		int32 Minutes;
		int32 Hours;
		int32 Days;
		int32 Months;
		int32 Years;
	};

	template<>
	struct NEXUS_FRAMEWORK_API StringConverter<Timespan>
	{
		static StringView GetFormat(bool Pretty);
		static void ToString(const Timespan& Data, String& Result, StringView Format = "");
		static void FromString(StringView Data, Timespan& Result, StringView Format = "");
	};

	NEXUS_FRAMEWORK_API bool operator==(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator!=(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator<(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator>(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator<=(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator>=(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API Timespan operator+(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API Timespan operator-(const Timespan& A, const Timespan& B);
}
