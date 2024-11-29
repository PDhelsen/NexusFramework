#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Strings/StringCApi.h"

namespace NxEn
{
	class StringUtility
	{
	public:
		enum class SearchMode
		{
			Substring, Characters
		};

		template<typename... Args>
		static void Format(String& Text, StringView Format, Args&&... args);
		template<typename... Args>
		static void Format(String& Text, uint64 Size, StringView Format, Args&&... args);
		template<typename... Args>
		static String Format(StringView Format, Args&&... args);
		template<typename... Args>
		static String Format(uint64 Size, StringView Format, Args&&... args);
		template<typename... Args>
		static uint64 Scan(StringView Text, StringView Format, Args&&... args);

		NEXUS_ENGINE_API static bool Start(StringView Text, StringView Substring);
		NEXUS_ENGINE_API static bool End(StringView Text, StringView Substring);
		NEXUS_ENGINE_API static bool Contains(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static StringView Common(StringView Text1, StringView Text2);
		NEXUS_ENGINE_API static StringView Find(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<StringView> FindAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static StringView Split(StringView Text, StringView Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<StringView> SplitAll(StringView Text, StringView Substring, SearchMode Mode = SearchMode::Substring);

		NEXUS_ENGINE_API static  int64 ToInteger(StringView Text, int32 Radix = 10);
		NEXUS_ENGINE_API static uint64 ToUnsignedInteger(StringView Text, int32 Radix = 10);
		NEXUS_ENGINE_API static double ToDouble(StringView Text);

		NEXUS_ENGINE_API static String ToStringI(int64 Number, StringView Format = "%d");
		NEXUS_ENGINE_API static String ToStringU(uint64 Number, StringView Format = "%d");
		NEXUS_ENGINE_API static String ToStringF(float Number, StringView Format = "%.2f");
		NEXUS_ENGINE_API static String ToStringD(double Number, StringView Format = "%.2f");
		NEXUS_ENGINE_API static String ToStringB(bool State, StringView Format = "%s");

		NEXUS_ENGINE_API static const char NullChar = StringCApi::NullChar;

	private:
		enum class SearchBehaviour
		{
			Contains, Find, Split
		};

		static StringView Search(const char* Text, const char* Substring, uint64 Size, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<StringView>* Results);

		static const uint8 GuessFormatingSize = 8;
	};

	NEXUS_ENGINE_API String operator+(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API String operator-(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API bool operator==(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API bool operator!=(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API bool operator>(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API bool operator>=(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API bool operator<(StringView TextA, StringView TextB);
	NEXUS_ENGINE_API bool operator<=(StringView TextA, StringView TextB);
}
