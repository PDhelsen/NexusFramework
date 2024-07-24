#pragma once

#include "Misc/Hash.h"
#include "Types/String.h"
#include "Types/StringFunctions.h"
#include "Types/StringCApi.h"

namespace NxEn
{
	template<typename... Args>
	void String::Format(const StringView& Format, Args&&... args)
	{
		StringUtility::Format(*this, Format, args...);
	}

	template<typename... Args>
	void String::Format(uint64 Size, const StringView& Format, Args&&... args)
	{
		StringUtility::Format(*this, Format, args...);
	}

	template<typename... Args>
	void String::Scan(const StringView& Format, Args&&... args)
	{
		StringUtility::Scan(*this, Format, args...);
	}

	template<typename ...Args>
	void StringUtility::Format(String& Text, const StringView& Format, Args && ...args)
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
	void StringUtility::Format(String& Text, uint64 Size, const StringView& Format, Args && ...args)
	{
		Text.Resize(Size);
		StringCApi::Format(Text.GetCapacity(), Text.GetData(), Format.C(), args...);
		Text.Validate();
	}

	template<typename... Args>
	static String StringUtility::Format(const StringView& Format, Args&&... args)
	{
		String Result = String(Format.GetCount() + sizeof...(args) * GuessFormatingSize);
		Result.Format(Format, args...);
		return Move(Result);
	}

	template<typename... Args>
	static String StringUtility::Format(uint64 Size, const StringView& Format, Args&&... args)
	{
		String Result = String(Size);
		Result.Format(Size, Format, args...);
		return Move(Result);
	}

	template<typename... Args>
	static void StringUtility::Scan(const StringView& Text, const StringView& Format, Args&&... args)
	{
		StringCApi::Scan(Text.C(), Format.C(), args...);
	}

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
		static H::HashLength HashObject(const StringView& Data, H::HashLength Seed = 0)
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
