#pragma once

#include "Misc/Hash.h"
#include "Misc/References.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringFunctions.h"
#include "Types/Strings/StringCApi.h"

namespace NxEn
{
	template<typename... Args>
	void String::Format(StringView Format, Args&&... args)
	{
		StringUtility::Format(*this, Format, args...);
	}

	template<typename... Args>
	void String::Format(uint64 Size, StringView Format, Args&&... args)
	{
		StringUtility::Format(*this, Format, args...);
	}

	template<typename... Args>
	uint64 String::Scan(StringView Format, Args&&... args)
	{
		return StringUtility::Scan(*this, Format, args...);
	}

	template<typename ...Args>
	void StringUtility::Format(String& Text, StringView Format, Args && ...args)
	{
		uint64 Size = StringCApi::Format(Text.GetCapacity(), Text.GetData(), Format.C(), args...);
		if (Size >= Text.GetCapacity())
		{
			Text.Resize(Size);
			StringCApi::Format(Text.GetCapacity(), Text.GetData(), Format.C(), args...);
		}
		Text.Validate();
	}

	template<typename ...Args>
	void StringUtility::Format(String& Text, uint64 Size, StringView Format, Args && ...args)
	{
		Text.Resize(Size);
		StringCApi::Format(Text.GetCapacity(), Text.GetData(), Format.C(), args...);
		Text.Validate();
	}

	template<typename... Args>
	static String StringUtility::Format(StringView Format, Args&&... args)
	{
		String Result = String(Format.GetCount() + sizeof...(args) * GuessFormatingSize);
		Result.Format(Format, args...);
		return Move(Result);
	}

	template<typename... Args>
	static String StringUtility::Format(uint64 Size, StringView Format, Args&&... args)
	{
		String Result = String(Size);
		Result.Format(Size, Format, args...);
		return Move(Result);
	}

	template<typename... Args>
	static uint64 StringUtility::Scan(StringView Text, StringView Format, Args&&... args)
	{
		return StringCApi::Scan(Text.C(), Format.C(), args...);
	}

	template<class H>
	struct Hash<const char*, H>
	{
		static H::HashLength HashObject(const char* Data, H::HashLength Seed = 0)
		{
			return H::Hash(Data, StringCApi::Length(Data), Seed);
		}
	};

	template<class H>
	struct Hash<String, H>
	{
		static H::HashLength HashObject(const String& Data, H::HashLength Seed = 0)
		{
			H Hashing = H(Seed);
			Hashing.Accumulate(Data.C(), Data.GetCount());
			return Hashing.Hash();
		}
	};

	template<class H>
	struct Hash<StringView, H>
	{
		static H::HashLength HashObject(StringView Data, H::HashLength Seed = 0)
		{
			H Hashing = H(Seed);
			Hashing.Accumulate(Data.C(), Data.GetCount());
			return Hashing.Hash();
		}
	};

	template<class H>
	struct Hash<StringId, H>
	{
		static H::HashLength HashObject(const StringId& Data, H::HashLength Seed = 0)
		{
			return Data.GetId();
		}
	};
}
