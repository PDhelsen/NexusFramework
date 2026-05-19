#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	class StringCApi
	{
	public:
		inline static const char NullChar = '\0';
		inline static const char NewLineChar = '\n';
		inline static const char* NewLine = "\n";

		NX_FRAMEWORK_API static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe);
		NX_FRAMEWORK_API static const char* SearchStr(const char* Source, const char* Substring, uint64 Capacity, uint64 Size);
		NX_FRAMEWORK_API static uint64 Common(const char* Text1, const char* Text2);

		NX_FRAMEWORK_API static uint64 Length(const char* Text);
		NX_FRAMEWORK_API static int8 Compare(const char* Text1, const char* Text2);
		NX_FRAMEWORK_API static int8 Compare(const char* Text1, const char* Text2, uint64 Size);
		NX_FRAMEWORK_API static void Copy(const char* Source, char* Destination, uint64 Capacity);
		NX_FRAMEWORK_API static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NX_FRAMEWORK_API static void Concat(const char* Source, char* Destination, uint64 Capacity);
		NX_FRAMEWORK_API static void Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NX_FRAMEWORK_API static const char* SearchStr(const char* Source, const char* Substring);
		NX_FRAMEWORK_API static const char* SearchChr(const char* Source, const char* Substring);
		NX_FRAMEWORK_API static uint64 Format(uint64 Capacity, char* Text, const char* Format, ...);
		NX_FRAMEWORK_API static uint64 Scan(const char* Text, const char* Format, ...);

		NX_FRAMEWORK_API static  int64 ToInteger(const char* Text, int32 Radix = 10);
		NX_FRAMEWORK_API static uint64 ToUnsignedInteger(const char* Text, int32 Radix = 10);
		NX_FRAMEWORK_API static double ToDouble(const char* Text);
	};
}
