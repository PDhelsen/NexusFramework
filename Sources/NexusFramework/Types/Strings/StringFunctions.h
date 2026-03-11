#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

namespace NxFr
{
	template <typename T> class Collection;

	class StringUtility
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
		static String Format(StringView Format, Args&&... args);
		template<typename... Args>
		static String Format(uint64 Size, StringView Format, Args&&... args);
		template<typename... Args>
		static uint64 Scan(StringView Text, StringView Format, Args&&... args);

		template<typename T>
		static String ToString(const T& Data, StringView Format = "");
		template<typename T>
		static void ToString(const T& Data, String& Result, StringView Format = "");
		template<typename T>
		static T FromString(StringView Data, StringView Format = "");
		template<typename T>
		static void FromString(StringView Data, T& Result, StringView Format = "");
		template<typename T>
		static StringView ConvertionFormat(StringView Format = "", bool Pretty = false);

		NEXUS_FRAMEWORK_API static bool Start(StringView Text, StringView Substring);
		NEXUS_FRAMEWORK_API static bool End(StringView Text, StringView Substring);
		NEXUS_FRAMEWORK_API static bool Contains(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static StringView Common(StringView Text1, StringView Text2);
		NEXUS_FRAMEWORK_API static StringView Find(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static List<StringView> FindAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static StringView Split(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static List<StringView> SplitAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static StringView TrimLeading(StringView Text, char Character = ' ');
		NEXUS_FRAMEWORK_API static StringView TrimTrailing(StringView Text, char Character = ' ');
		NEXUS_FRAMEWORK_API static String Lower(StringView Text);
		NEXUS_FRAMEWORK_API static String Upper(StringView Text);
		NEXUS_FRAMEWORK_API static String Replace(StringView Text, StringView Old, StringView New);
		NEXUS_FRAMEWORK_API static String Join(const Collection<StringView>& Text, StringView Separator = "");

	private:
		static StringView Search(const char* Text, const char* Substring, uint64 Capacity, uint64 Size, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<StringView>* Results);
	};

	template<typename T>
	struct StringConverter
	{
		static StringView GetFormat(bool Pretty) { return ""; };
		static void ToString(const T& Data, String& Result, StringView Format = "");
		static void FromString(StringView Data, T& Result, StringView Format = "");
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
