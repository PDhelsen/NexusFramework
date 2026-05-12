#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

namespace NxFr
{
	template <typename T> class Collection;

	template<typename T>
	struct StringConverter
	{
		static StringView GetFormat(bool Pretty) { return ""; };
		static void ToString(const T& Data, String& Result, StringView Format = "") {}
		static void FromString(StringView Data, T& Result, StringView Format = "") {}
	};

	class NEXUS_FRAMEWORK_API StringUtility
	{
	public:
		enum class SearchMode
		{
			Substring, Characters
		};

	private:
		enum class SearchBehaviour
		{
			Contains, Find, Split
		};

	public:
		inline static const char NullChar = StringCApi::NullChar;
		inline static const char NewLineChar = StringCApi::NewLineChar;
		inline static const String NewLine = StringCApi::NewLine;
		inline static const String Empty = "";
		inline static const String Unknown = "Unknown";
		inline static const StringId Id = 0;

	private:
		static const uint8 GuessFormatingSize = 8;

	public:
		template<typename... Args>
		static void Format(String& Text, StringView Formatting, Args&&... args)
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
		static void Format(String& Text, uint64 Size, StringView Formatting, Args&&... args)
		{
			Text.Reserve(Size);
			StringCApi::Format(Text.GetCapacity(), Text.Characters(), Formatting.C(), args...);
			Text.Validate();
		}
		template<typename... Args>
		static String FormatTo(StringView Formatting, Args&&... args)
		{
			String Result = String(Formatting.GetCount() + sizeof...(args) * GuessFormatingSize);
			Format(Result, Formatting, args...);
			return Result;
		}
		template<typename... Args>
		static String FormatTo(uint64 Size, StringView Formatting, Args&&... args)
		{
			String Result = String(Size);
			Format(Result, Size, Formatting, args...);
			return Result;
		}
		template<typename... Args>
		static uint64 Scan(StringView Text, StringView Formatting, Args&&... args)
		{
			return StringCApi::Scan(Text.C(), Formatting.C(), args...);
		}

		template<typename T>
		static String ToString(const T& Data, StringView Format = "")
		{
			String Result;
			ToString<T>(Data, Result, Format);
			return Result;
		}
		template<typename T>
		static void ToString(const T& Data, String& Result, StringView Format = "")
		{
			StringConverter<T>::ToString(Data, Result, Format);
		}
		template<typename T>
		static T FromString(StringView Data, StringView Format = "")
		{
			T Result;
			FromString<T>(Data, Result, Format);
			return Result;
		}
		template<typename T>
		static void FromString(StringView Data, T& Result, StringView Format = "")
		{
			StringConverter<T>::FromString(Data, Result, Format);
		}
		template<typename T>
		static StringView ConvertionFormat(StringView Format = "", bool Pretty = false)
		{
			return !Format.IsEmpty() ? Format : StringConverter<T>::GetFormat(Pretty);
		}

		static bool Start(StringView Text, StringView Substring);
		static bool End(StringView Text, StringView Substring);
		static bool Contains(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		static StringView Common(StringView Text1, StringView Text2);
		static StringView Find(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		static List<StringView> FindAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		static StringView Split(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		static List<StringView> SplitAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		static StringView TrimLeading(StringView Text, char Character = ' ');
		static StringView TrimTrailing(StringView Text, char Character = ' ');
		static String Lower(StringView Text);
		static String Upper(StringView Text);
		static String Replace(StringView Text, StringView Old, StringView New);
		static String Join(const Collection<StringView>& Text, StringView Separator = "");

	private:
		static StringView Search(const char* Text, const char* Substring, uint64 Capacity, uint64 Size, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<StringView>* Results);
	};

	NEXUS_FRAMEWORK_API String operator+(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API String operator-(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API bool operator==(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API bool operator!=(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API bool operator>(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API bool operator>=(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API bool operator<(StringView TextA, StringView TextB);
	NEXUS_FRAMEWORK_API bool operator<=(StringView TextA, StringView TextB);
}
