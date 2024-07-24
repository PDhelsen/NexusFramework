#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Strings/StringId.h"
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
		static void Format(String& Text, const StringView& Format, Args&&... args);
		template<typename... Args>
		static void Format(String& Text, uint64 Size, const StringView& Format, Args&&... args);
		template<typename... Args>
		static String Format(const StringView& Format, Args&&... args);
		template<typename... Args>
		static String Format(uint64 Size, const StringView& Format, Args&&... args);
		template<typename... Args>
		static void Scan(const StringView& Text, const StringView& Format, Args&&... args);

		NEXUS_ENGINE_API static bool Start(const StringView& Text, const StringView& Substring);
		NEXUS_ENGINE_API static bool End(const StringView& Text, const StringView& Substring);
		NEXUS_ENGINE_API static bool Contains(const StringView& Text, const StringView& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static StringView Find(const StringView& Text, const StringView& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<StringView> FindAll(const StringView& Text, const StringView& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static StringView Split(const StringView& Text, const StringView& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<StringView> SplitAll(const StringView& Text, const StringView& Substring, SearchMode Mode = SearchMode::Substring);

		NEXUS_ENGINE_API static  int64 ToInteger(const StringView& Text, int32 Radix = 10);
		NEXUS_ENGINE_API static uint64 ToUnsignedInteger(const StringView& Text, int32 Radix = 10);
		NEXUS_ENGINE_API static double ToDouble(const StringView& Text);

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

	NEXUS_ENGINE_API String operator+(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API String operator-(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API bool operator==(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API bool operator!=(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API bool operator>(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API bool operator>=(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API bool operator<(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API bool operator<=(const StringView& TextA, const StringView& TextB);
	NEXUS_ENGINE_API StringId operator""_Sid(const char* Text, uint64 Size);
}

using NxEn::operator""_Sid;
