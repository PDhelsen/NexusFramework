#include "Core/NexusEnginePch.h"
#include "StringCApi.h"

namespace NxEn
{
	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Destination, Default, "Invalid Destination");
		NEXUS_ASSERT(Capacity > 0, Default, "Invalid Capacity");

		if (NotSafe)
		{
#pragma warning(suppress : 4996)
			strncpy(Destination, Source, Size);
		}
		else
		{
			StringCApi::Copy(Source, Destination, Capacity, Size);
		}
	}

	const char* StringCApi::SearchStr(const char* Source, const char* Substring, uint64 Size)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Substring, Default, "Invalid Substring");

		uint64 Count = 0;
		bool Finding = false;
		const char* Result = nullptr;
		const char* Current = Source;
		const char* Target = Substring;

		while (*Current != NullChar)
		{
			if (*Current == *Target)
			{
				if (!Finding)
				{
					Count = 0;
					Finding = true;
					Result = Current;
				}

				++Target;
				++Count;

				if (Count == Size)
				{
					break;
				}
			}
			else
			{
				Target = Substring;
				Result = nullptr;
				Finding = false;
			}

			++Current;
		}

		if (Count != Size)
		{
			Result = nullptr;
		}

		return Result;
	}

	uint64 StringCApi::Common(const char* Text1, const char* Text2)
	{
		NEXUS_ASSERT(Text1, Default, "Invalid Text1");
		NEXUS_ASSERT(Text2, Default, "Invalid Text2");

		uint64 Count = 0;
		while (*Text1 == *Text2 && *Text1 != NullChar && *Text2 != NullChar)
		{
			Text1++;
			Text2++;
			Count++;
		}
		return Count;
	}

	uint64 StringCApi::Length(const char* Text)
	{
		NEXUS_ASSERT(Text, Default, "Invalid Text");

		return strlen(Text);
	}

	int8 StringCApi::Compare(const char* Text1, const char* Text2)
	{
		NEXUS_ASSERT(Text1, Default, "Invalid Text1");
		NEXUS_ASSERT(Text2, Default, "Invalid Text2");

		return strcmp(Text1, Text2);
	}

	int8 StringCApi::Compare(const char* Text1, const char* Text2, uint64 Size)
	{
		NEXUS_ASSERT(Text1, Default, "Invalid Text1");
		NEXUS_ASSERT(Text2, Default, "Invalid Text2");

		return strncmp(Text1, Text2, Size);
	}

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Destination, Default, "Invalid Destination");
		NEXUS_ASSERT(Capacity > 0, Default, "Invalid Capacity");

		strcpy_s(Destination, Capacity, Source);
	}

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Destination, Default, "Invalid Destination");
		NEXUS_ASSERT(Capacity > 0, Default, "Invalid Capacity");

		strncpy_s(Destination, Capacity, Source, Size);
	}

	void StringCApi::Concat(const char* Source, char* Destination, uint64 Capacity)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Destination, Default, "Invalid Destination");
		NEXUS_ASSERT(Capacity > 0, Default, "Invalid Capacity");

		strcat_s(Destination, Capacity, Source);
	}

	void StringCApi::Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Destination, Default, "Invalid Destination");
		NEXUS_ASSERT(Capacity > 0, Default, "Invalid Capacity");

		strncat_s(Destination, Capacity, Source, Size);
	}

	const char* StringCApi::SearchStr(const char* Source, const char* Substring)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Substring, Default, "Invalid Substring");

		return strstr(Source, Substring);
	}

	const char* StringCApi::SearchChr(const char* Source, const char* Substring)
	{
		NEXUS_ASSERT(Source, Default, "Invalid Source");
		NEXUS_ASSERT(Substring, Default, "Invalid Substring");

		return strpbrk(Source, Substring);
	}

	uint64 StringCApi::Format(uint64 Capacity, char* Text, const char* Format, ...)
	{
		NEXUS_ASSERT(Text, Default, "Invalid Text");
		NEXUS_ASSERT(Format, Default, "Invalid Format");
		NEXUS_ASSERT(Capacity > 0, Default, "Invalid Capacity");

		NEXUS_VA(Format, uint64 Size = vsnprintf(Text, Capacity, Format, ArgList));
		return Size;
	}

	uint64 StringCApi::Scan(const char* Text, const char* Format, ...)
	{
		NEXUS_ASSERT(Text, Default, "Invalid Text");
		NEXUS_ASSERT(Format, Default, "Invalid Format");

		NEXUS_VA(Format, uint64 Count = vsscanf(Text, Format, ArgList));
		return Count;
	}

	int64 StringCApi::ToInteger(const char* Text, int32 Radix)
	{
		NEXUS_ASSERT(Text, Default, "Invalid Text");

		char* Ptr; return strtol(Text, &Ptr, Radix);
	}

	uint64 StringCApi::ToUnsignedInteger(const char* Text, int32 Radix)
	{
		NEXUS_ASSERT(Text, Default, "Invalid Text");

		char* Ptr; return strtoul(Text, &Ptr, Radix);
	}

	double StringCApi::ToDouble(const char* Text)
	{
		NEXUS_ASSERT(Text, Default, "Invalid Text");

		char* Ptr; return strtod(Text, &Ptr);
	}
}
