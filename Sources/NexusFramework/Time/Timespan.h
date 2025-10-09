#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"

namespace NxFr
{
	struct Timespan
	{
	public:
		NEXUS_FRAMEWORK_API Timespan();
		NEXUS_FRAMEWORK_API Timespan(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S);

		NEXUS_FRAMEWORK_API Timespan& operator+=(const Timespan& Other);
		NEXUS_FRAMEWORK_API Timespan& operator-=(const Timespan& Other);

		NEXUS_FRAMEWORK_API void Normalize();

		NEXUS_FRAMEWORK_API bool IsValid() const;

		int32 Seconds;
		int32 Minutes;
		int32 Hours;
		int32 Days;
		int32 Months;
		int32 Years;
	};

	template<>
	struct StringConverter<Timespan>
	{
		NEXUS_FRAMEWORK_API static StringView GetFormat(bool Pretty);
		NEXUS_FRAMEWORK_API static void ToString(const Timespan& Data, String& Result, StringView Format = "");
		NEXUS_FRAMEWORK_API static void FromString(StringView Data, Timespan& Result, StringView Format = "");
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
