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
		StringUtility::Format(*this, Size, Format, args...);
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

	namespace Hashing
	{
		template<typename H>
		class HashProcess<const char*, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const char* Data)
			{
				State.Accumulate(Data, StringCApi::Length(Data));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const char* Data)
			{
				HashProcess<const char*, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<String, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const String& Data)
			{
				State.Accumulate(Data.C(), Data.GetCount());
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const String& Data)
			{
				HashProcess<String, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<StringView, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const StringView& Data)
			{
				State.Accumulate(Data.C(), Data.GetCount());
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const StringView& Data)
			{
				HashProcess<StringView, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<StringId, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const StringId& Data)
			{
				State.Accumulate(Data.C(), StringCApi::Length(Data.C()));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const StringId& Data)
			{
				return Data.GetId();
			}
		};
	}
}
