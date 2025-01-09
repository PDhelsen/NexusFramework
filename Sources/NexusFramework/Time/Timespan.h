#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Types/Numbers/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxFr
{
	struct Timespan
	{
	public:
		inline static const String Format = "%Yy%mm%dd - %Hh%Mm%Ss";

		NEXUS_FRAMEWORK_API Timespan();
		NEXUS_FRAMEWORK_API Timespan(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S);

		NEXUS_FRAMEWORK_API Timespan& operator+=(const Timespan& Other);
		NEXUS_FRAMEWORK_API Timespan& operator-=(const Timespan& Other);

		NEXUS_FRAMEWORK_API void Normalize();

		NEXUS_FRAMEWORK_API String ToString(StringView Format = Format) const;

		NEXUS_FRAMEWORK_API bool IsValid() const;

		int32 Seconds;
		int32 Minutes;
		int32 Hours;
		int32 Days;
		int32 Months;
		int32 Years;
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
