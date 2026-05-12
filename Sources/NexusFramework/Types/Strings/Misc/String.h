#pragma once

#include "NexusFramework/Types/Strings/Utility.h"

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Numeric/Decimal.h"

namespace NxFr
{
	template<>
	struct StringConverter<const char*>
	{
		static void ToString(const char* Data, String& Result, StringView Format = "")
		{
			Result = Data;
		}

		static void FromString(StringView Data, const char* Result, StringView Format = "")
		{
			Result = Data.C();
		}
	};

	template<>
	struct StringConverter<StringView>
	{
		static void ToString(const StringView& Data, String& Result, StringView Format = "")
		{
			Result = Data;
		}

		static void FromString(StringView Data, StringView& Result, StringView Format = "")
		{
			Result = Data;
		}
	};

	template<>
	struct StringConverter<String>
	{
		static void ToString(const String& Data, String& Result, StringView Format = "")
		{
			Result = Data;
		}

		static void FromString(StringView Data, String& Result, StringView Format = "")
		{
			Result = Data;
		}
	};

	template<>
	struct StringConverter<StringId>
	{
		static void ToString(const StringId& Data, String& Result, StringView Format = "")
		{
			Result = Data.GetString();
		}

		static void FromString(StringView Data, StringId& Result, StringView Format = "")
		{
			Result = StringId(Data);
		}
	};

	template<>
	struct StringConverter<bool>
	{
		static void ToString(const bool& Data, String& Result, StringView Format = "")
		{
			Result = Data ? "true" : "false";
		}

		static void FromString(StringView Data, bool& Result, StringView Format = "")
		{
			Result = Data == "true" || Data == "True" || Data == "TRUE" ? true : false;
		}
	};

	template<>
	struct StringConverter<int8>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%hhd";
		}

		static void ToString(const int8& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<int8>(Format), Data);
		}

		static void FromString(StringView Data, int8& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<int8>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<int16>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%hd";
		}

		static void ToString(const int16& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<int16>(Format), Data);
		}

		static void FromString(StringView Data, int16& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<int16>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<int32>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%d";
		}

		static void ToString(const int32& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<int32>(Format), Data);
		}

		static void FromString(StringView Data, int32& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<int32>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<int64>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%lld";
		}

		static void ToString(const int64& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<int64>(Format), Data);
		}

		static void FromString(StringView Data, int64& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<int64>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<uint8>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%hhu";
		}

		static void ToString(const uint8& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<uint8>(Format), Data);
		}

		static void FromString(StringView Data, uint8& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<uint8>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<uint16>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%hu";
		}

		static void ToString(const uint16& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<uint16>(Format), Data);
		}

		static void FromString(StringView Data, uint16& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<uint16>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<uint32>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%u";
		}

		static void ToString(const uint32& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<uint32>(Format), Data);
		}

		static void FromString(StringView Data, uint32& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<uint32>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<uint64>
	{
		static StringView GetFormat(bool Pretty)
		{
			return "%llu";
		}

		static void ToString(const uint64& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<uint64>(Format), Data);
		}

		static void FromString(StringView Data, uint64& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<uint64>(Format), &Result);
		}
	};

	template<>
	struct StringConverter<float>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "%.2f" : "%f";
		}

		static void ToString(const float& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<float>(Format, true), Data);
		}

		static void FromString(StringView Data, float& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<float>(Format, false), &Result);
		}
	};

	template<>
	struct StringConverter<double>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "%.2lf" : "%lf";
		}

		static void ToString(const double& Data, String& Result, StringView Format = "")
		{
			StringUtility::Format(Result, StringUtility::ConvertionFormat<double>(Format, true), Data);
		}

		static void FromString(StringView Data, double& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<double>(Format, false), &Result);
		}
	};
}
