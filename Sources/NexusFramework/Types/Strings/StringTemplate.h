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
}
