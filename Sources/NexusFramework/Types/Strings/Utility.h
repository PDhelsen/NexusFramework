#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

namespace NxFr
{
	template <typename T> class List;
	template <typename T> class Collection;

	namespace StringUtility
	{
		enum class SearchMode
		{
			Substring, Characters
		};

		template<typename T>
		struct Converter
		{
			static StringView GetFormat(bool Pretty) { return ""; };
			static void ToString(const T& Data, String& Result, StringView Format = "") {}
			static void FromString(StringView Data, T& Result, StringView Format = "") {}
		};

		inline static const uint8 GuessFormatingSize = 8;
		inline static const char NullChar = StringCApi::NullChar;
		inline static const char NewLineChar = StringCApi::NewLineChar;
		inline static const String NewLine = StringCApi::NewLine;
		inline static const String Empty = "";
		inline static const String Unknown = "Unknown";
		inline static const StringId Id = 0;

		template<typename... Args>
		void Format(String& Text, StringView Formatting, Args&&... args)
		{
			uint64 Size = StringCApi::Format(Text.GetCapacity(), Text.Characters(), Formatting.C(), args...);
			if (Size >= Text.GetCapacity())
			{
				Text.Reserve(Size + 1);
				StringCApi::Format(Text.GetCapacity(), Text.Characters(), Formatting.C(), args...);
			}
			Text.Validate();
		}
		template<typename... Args>
		void Format(String& Text, uint64 Size, StringView Formatting, Args&&... args)
		{
			Text.Reserve(Size);
			StringCApi::Format(Text.GetCapacity(), Text.Characters(), Formatting.C(), args...);
			Text.Validate();
		}
		template<typename... Args>
		String FormatTo(StringView Formatting, Args&&... args)
		{
			String Result = String(Formatting.GetCount() + sizeof...(args) * GuessFormatingSize);
			Format(Result, Formatting, args...);
			return Result;
		}
		template<typename... Args>
		String FormatTo(uint64 Size, StringView Formatting, Args&&... args)
		{
			String Result = String(Size);
			Format(Result, Size, Formatting, args...);
			return Result;
		}
		template<typename... Args>
		uint64 Scan(StringView Text, StringView Formatting, Args&&... args)
		{
			return StringCApi::Scan(Text.C(), Formatting.C(), args...);
		}

		template<typename T>
		void ToString(const T& Data, String& Result, StringView Format = "")
		{
			Converter<T>::ToString(Data, Result, Format);
		}
		template<typename T>
		String ToString(const T& Data, StringView Format = "")
		{
			String Result;
			ToString<T>(Data, Result, Format);
			return Result;
		}
		template<typename T>
		void FromString(StringView Data, T& Result, StringView Format = "")
		{
			Converter<T>::FromString(Data, Result, Format);
		}
		template<typename T>
		T FromString(StringView Data, StringView Format = "")
		{
			T Result;
			FromString<T>(Data, Result, Format);
			return Result;
		}

		template<typename T>
		StringView ConvertionFormat(StringView Format = "", bool Pretty = false)
		{
			return !Format.IsEmpty() ? Format : Converter<T>::GetFormat(Pretty);
		}

		NX_FRAMEWORK_API bool Start(StringView Text, StringView Substring);
		NX_FRAMEWORK_API bool End(StringView Text, StringView Substring);
		NX_FRAMEWORK_API bool Contains(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NX_FRAMEWORK_API StringView Common(StringView Text1, StringView Text2);
		NX_FRAMEWORK_API StringView Find(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NX_FRAMEWORK_API List<StringView> FindAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NX_FRAMEWORK_API StringView Split(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NX_FRAMEWORK_API List<StringView> SplitAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NX_FRAMEWORK_API List<StringView> Tokenize(StringView Text, StringView Separator = " ");
		NX_FRAMEWORK_API StringView TrimLeading(StringView Text, char Character = ' ');
		NX_FRAMEWORK_API StringView TrimTrailing(StringView Text, char Character = ' ');
		NX_FRAMEWORK_API String Lower(StringView Text);
		NX_FRAMEWORK_API String Upper(StringView Text);
		NX_FRAMEWORK_API String Replace(StringView Text, StringView Old, StringView New);
		NX_FRAMEWORK_API String Join(const Collection<StringView>& Text, StringView Separator = "");
	};

	NX_FRAMEWORK_API String operator+(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API String operator-(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API bool operator==(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API bool operator!=(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API bool operator>(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API bool operator>=(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API bool operator<(StringView TextA, StringView TextB);
	NX_FRAMEWORK_API bool operator<=(StringView TextA, StringView TextB);
}
