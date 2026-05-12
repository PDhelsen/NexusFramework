#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/Utility.h"

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

	NEXUS_FRAMEWORK_API bool operator==(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator!=(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator<(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator>(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator<=(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API bool operator>=(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API Timespan operator+(const Timespan& A, const Timespan& B);
	NEXUS_FRAMEWORK_API Timespan operator-(const Timespan& A, const Timespan& B);
}
