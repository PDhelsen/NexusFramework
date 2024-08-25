#include "Core/NexusEnginePch.h"
#include "StringCApi.h"

namespace NxEn
{
	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe)
	{
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

				Target++;
				Count++;

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

			Current++;
		}

		if (Count != Size)
		{
			Result = nullptr;
		}

		return Result;
	}

	uint64 StringCApi::Length(const char* Text)
	{
		return strlen(Text);
	}

	int8 StringCApi::Compare(const char* Text1, const char* Text2)
	{
		return strcmp(Text1, Text2);
	}

	int8 StringCApi::Compare(const char* Text1, const char* Text2, uint64 Size)
	{
		return strncmp(Text1, Text2, Size);
	}

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity)
	{
		strcpy_s(Destination, Capacity, Source);
	}

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size)
	{
		strncpy_s(Destination, Capacity, Source, Size);
	}

	void StringCApi::Concat(const char* Source, char* Destination, uint64 Capacity)
	{
		strcat_s(Destination, Capacity, Source);
	}

	void StringCApi::Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size)
	{
		strncat_s(Destination, Capacity, Source, Size);
	}

	const char* StringCApi::SearchStr(const char* Source, const char* Substring)
	{
		return strstr(Source, Substring);
	}

	const char* StringCApi::SearchChr(const char* Source, const char* Substring)
	{
		return strpbrk(Source, Substring);
	}

	uint64 StringCApi::Format(uint64 Capacity, char* Text, const char* Format, ...)
	{
		NEXUS_VA(Format, uint64 Size = vsnprintf(Text, Capacity, Format, ArgList));
		return Size;
	}

	uint64 StringCApi::Scan(const char* Text, const char* Format, ...)
	{
		NEXUS_VA(Format, uint64 Count = vsscanf(Text, Format, ArgList));
		return Count;
	}

	int64 StringCApi::ToInteger(const char* Text, int32 Radix)
	{
		char* Ptr; return strtol(Text, &Ptr, Radix);
	}

	uint64 StringCApi::ToUnsignedInteger(const char* Text, int32 Radix)
	{
		char* Ptr; return strtoul(Text, &Ptr, Radix);
	}

	double StringCApi::ToDouble(const char* Text)
	{
		char* Ptr; return strtod(Text, &Ptr);
	}

	uint64 StringCApi::ToStringI(int64 Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringU(uint64 Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringF(float Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringD(double Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringB(bool State, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, State ? "True" : "False");
	}
}
