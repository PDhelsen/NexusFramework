#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Types/Numbers/Integer.h"

namespace NxEn
{
	class StringCApi
	{
	public:
		inline static const char NullChar = '\0';
		inline static const char NewLineChar = '\n';
		inline static const char* NewLine = "\n";

		NEXUS_FRAMEWORK_API static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe);
		NEXUS_FRAMEWORK_API static const char* SearchStr(const char* Source, const char* Substring, uint64 Size);
		NEXUS_FRAMEWORK_API static uint64 Common(const char* Text1, const char* Text2);

		NEXUS_FRAMEWORK_API static uint64 Length(const char* Text);
		NEXUS_FRAMEWORK_API static int8 Compare(const char* Text1, const char* Text2);
		NEXUS_FRAMEWORK_API static int8 Compare(const char* Text1, const char* Text2, uint64 Size);
		NEXUS_FRAMEWORK_API static void Copy(const char* Source, char* Destination, uint64 Capacity);
		NEXUS_FRAMEWORK_API static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NEXUS_FRAMEWORK_API static void Concat(const char* Source, char* Destination, uint64 Capacity);
		NEXUS_FRAMEWORK_API static void Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NEXUS_FRAMEWORK_API static const char* SearchStr(const char* Source, const char* Substring);
		NEXUS_FRAMEWORK_API static const char* SearchChr(const char* Source, const char* Substring);
		NEXUS_FRAMEWORK_API static uint64 Format(uint64 Capacity, char* Text, const char* Format, ...);
		NEXUS_FRAMEWORK_API static uint64 Scan(const char* Text, const char* Format, ...);

		NEXUS_FRAMEWORK_API static  int64 ToInteger(const char* Text, int32 Radix = 10);
		NEXUS_FRAMEWORK_API static uint64 ToUnsignedInteger(const char* Text, int32 Radix = 10);
		NEXUS_FRAMEWORK_API static double ToDouble(const char* Text);
	};
}
