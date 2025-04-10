#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

namespace NxFr
{
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

	private:
		static const uint8 GuessFormatingSize = 8;

	public:
		template<typename... Args>
		static String Format(StringView Format, Args&&... args);
		template<typename... Args>
		static String Format(uint64 Size, StringView Format, Args&&... args);
		template<typename... Args>
		static void Format(String& Text, StringView Format, Args&&... args);
		template<typename... Args>
		static void Format(String& Text, uint64 Size, StringView Format, Args&&... args);
		template<typename... Args>
		static uint64 Scan(StringView Text, StringView Format, Args&&... args);

		NEXUS_FRAMEWORK_API static bool Start(StringView Text, StringView Substring);
		NEXUS_FRAMEWORK_API static bool End(StringView Text, StringView Substring);
		NEXUS_FRAMEWORK_API static bool Contains(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static StringView Common(StringView Text1, StringView Text2);
		NEXUS_FRAMEWORK_API static StringView Find(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static List<StringView> FindAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static StringView Split(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_FRAMEWORK_API static List<StringView> SplitAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);

		NEXUS_FRAMEWORK_API static  int64 ToInteger(StringView Text, int32 Radix = 10);
		NEXUS_FRAMEWORK_API static uint64 ToUnsignedInteger(StringView Text, int32 Radix = 10);
		NEXUS_FRAMEWORK_API static double ToDouble(StringView Text);

		NEXUS_FRAMEWORK_API static String ToStringI(int64 Number, StringView Format = "%d");
		NEXUS_FRAMEWORK_API static String ToStringU(uint64 Number, StringView Format = "%d");
		NEXUS_FRAMEWORK_API static String ToStringF(float Number, StringView Format = "%.2f");
		NEXUS_FRAMEWORK_API static String ToStringD(double Number, StringView Format = "%.2f");
		NEXUS_FRAMEWORK_API static String ToStringB(bool State, StringView Format = "%s");

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
