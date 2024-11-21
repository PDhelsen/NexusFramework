#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxEn
{
	struct Timespan
	{
	public:
		NEXUS_ENGINE_API Timespan();
		NEXUS_ENGINE_API Timespan(int32 Y, int32 M, int32 D, int32 H, int32 Mn, int32 S);

		NEXUS_ENGINE_API Timespan& operator+=(const Timespan& Other);
		NEXUS_ENGINE_API Timespan& operator-=(const Timespan& Other);

		NEXUS_ENGINE_API void Normalize();

		NEXUS_ENGINE_API String ToString(StringView Format = Format) const;

		NEXUS_ENGINE_API bool IsValid() const;

		inline static const StringView Format = "%Yy%mm%dd - %Hh%Mm%Ss";

		int32 Seconds;
		int32 Minutes;
		int32 Hours;
		int32 Days;
		int32 Months;
		int32 Years;
	};

	NEXUS_ENGINE_API bool operator==(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API bool operator!=(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API bool operator<(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API bool operator>(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API bool operator<=(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API bool operator>=(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API Timespan operator+(const Timespan& A, const Timespan& B);
	NEXUS_ENGINE_API Timespan operator-(const Timespan& A, const Timespan& B);
}
