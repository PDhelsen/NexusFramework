#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	struct NX_FRAMEWORK_API Timespan
	{
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

	NX_FRAMEWORK_API bool operator==(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API bool operator!=(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API bool operator<(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API bool operator>(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API bool operator<=(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API bool operator>=(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API Timespan operator+(const Timespan& A, const Timespan& B);
	NX_FRAMEWORK_API Timespan operator-(const Timespan& A, const Timespan& B);
}
