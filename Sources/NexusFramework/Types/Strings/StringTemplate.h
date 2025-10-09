#pragma once

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

namespace NxFr
{
	template<typename ...Args>
	inline String& String::Format(StringView Format, Args && ...args)
	{
		uint64 Size = StringCApi::Format(GetCapacity(), Characters(), Format.C(), args...);
		if (Size >= GetCapacity())
		{
			Resize(Size);
			StringCApi::Format(GetCapacity(), Characters(), Format.C(), args...);
		}
		Validate();
		return *this;
	}

	template<typename ...Args>
	inline String& String::Format(uint64 Size, StringView Format, Args && ...args)
	{
		Resize(Size);
		StringCApi::Format(GetCapacity(), Characters(), Format.C(), args...);
		Validate();
		return *this;
	}

	template<typename... Args>
	inline String StringUtility::Format(StringView Format, Args&&... args)
	{
		String Result = String(Format.GetCount() + sizeof...(args) * GuessFormatingSize);
		Result.Format(Format, args...);
		return Result;
	}

	template<typename... Args>
	inline String StringUtility::Format(uint64 Size, StringView Format, Args&&... args)
	{
		String Result = String(Size);
		Result.Format(Size, Format, args...);
		return Result;
	}

	template<typename... Args>
	inline uint64 StringUtility::Scan(StringView Text, StringView Format, Args&&... args)
	{
		return StringCApi::Scan(Text.C(), Format.C(), args...);
	}

	template<typename T>
	inline String StringUtility::ToString(const T& Data, StringView Format)
	{
		String Result;
		ToString<T>(Data, Result, Format);
		return Result;
	}

	template<typename T>
	inline void StringUtility::ToString(const T& Data, String& Result, StringView Format)
	{
		StringConverter<T>::ToString(Data, Result, Format);
	}

	template<typename T>
	inline T StringUtility::FromString(StringView Data, StringView Format)
	{
		T Result;
		FromString<T>(Data, Result, Format);
		return Result;
	}

	template<typename T>
	inline void StringUtility::FromString(StringView Data, T& Result, StringView Format)
	{
		StringConverter<T>::FromString(Data, Result, Format);
	}

	template<typename T>
	inline StringView StringUtility::ConvertionFormat(StringView Format, bool Pretty)
	{
		return !Format.IsEmpty() ? Format : StringConverter<T>::GetFormat(Pretty);
	}

	template<typename T>
	inline void StringConverter<T>::ToString(const T& Data, String& Result, StringView Format)
	{
	}

	template<typename T>
	inline void StringConverter<T>::FromString(StringView Data, T& Result, StringView Format)
	{
	}

#pragma region Convertion

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
			Result.Format(StringUtility::ConvertionFormat<int8>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<int16>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<int32>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<int64>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<uint8>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<uint16>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<uint32>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<uint64>(Format), Data);
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
			Result.Format(StringUtility::ConvertionFormat<float>(Format, true), Data);
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
			Result.Format(StringUtility::ConvertionFormat<double>(Format, true), Data);
		}

		static void FromString(StringView Data, double& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<double>(Format, false), &Result);
		}
	};

#pragma endregion
}
