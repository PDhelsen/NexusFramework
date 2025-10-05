#pragma once

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

namespace NxFr
{
	template<typename ...Args>
	String& String::Format(StringView Format, Args && ...args)
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
	String& String::Format(uint64 Size, StringView Format, Args && ...args)
	{
		Resize(Size);
		StringCApi::Format(GetCapacity(), Characters(), Format.C(), args...);
		Validate();
		return *this;
	}

	template<typename... Args>
	static String StringUtility::Format(StringView Format, Args&&... args)
	{
		String Result = String(Format.GetCount() + sizeof...(args) * GuessFormatingSize);
		Result.Format(Format, args...);
		return Result;
	}

	template<typename... Args>
	static String StringUtility::Format(uint64 Size, StringView Format, Args&&... args)
	{
		String Result = String(Size);
		Result.Format(Size, Format, args...);
		return Result;
	}

	template<typename... Args>
	static uint64 StringUtility::Scan(StringView Text, StringView Format, Args&&... args)
	{
		return StringCApi::Scan(Text.C(), Format.C(), args...);
	}
}
