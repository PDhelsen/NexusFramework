#pragma once

#include "Core/NexusEngineCore.h"
#include "Types/Integer.h"
#include "Types/Containers/List.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "External/StandardLibrary.h"

namespace NxEn
{
	class String
	{
	public:
		enum class SearchMode
		{
			Substring, Characters
		};

		NEXUS_ENGINE_API String();
		NEXUS_ENGINE_API String(uint64 Bytes);
		NEXUS_ENGINE_API String(const char* Text);
		NEXUS_ENGINE_API String(const String& Other);
		NEXUS_ENGINE_API String(String&& Other) noexcept;
		NEXUS_ENGINE_API ~String();

		NEXUS_ENGINE_API String& operator+=(const String& Other);
		NEXUS_ENGINE_API String& operator+=(const char* Other);
		NEXUS_ENGINE_API String& operator-=(const String& Other);
		NEXUS_ENGINE_API String& operator-=(const char* Other);

		NEXUS_ENGINE_API String& Append(const String& Text);
		NEXUS_ENGINE_API String& Append(const char* Text);
		NEXUS_ENGINE_API String& Assign(const String& OldText, const String& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Assign(const String& OldText, const char* NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Assign(const char* OldText, const String& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Assign(const char* OldText, const char* NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Insert(const String& ReferenceText, const String& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Insert(const String& ReferenceText, const char* NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Insert(const char* ReferenceText, const String& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Insert(const char* ReferenceText, const char* NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Remove(const String& Text, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Remove(const char* Text, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Clear();

		NEXUS_ENGINE_API void Grow(uint64 Size);
		NEXUS_ENGINE_API void Shrink(uint64 Size = 0);

		NEXUS_ENGINE_API String& Replace(const String& Old, const String& New);
		NEXUS_ENGINE_API String& Replace(const String& Old, const char* New);
		NEXUS_ENGINE_API String& Replace(const char* Old, const String& New);
		NEXUS_ENGINE_API String& Replace(const char* Old, const char* New);
		NEXUS_ENGINE_API int8 Compare(const String& Substring) const;
		NEXUS_ENGINE_API int8 Compare(const char* Substring) const;
		NEXUS_ENGINE_API bool Start(const String& Substring) const;
		NEXUS_ENGINE_API bool Start(const char* Substring) const;
		NEXUS_ENGINE_API bool End(const String& Substring) const;
		NEXUS_ENGINE_API bool End(const char* Substring) const;
		NEXUS_ENGINE_API bool Contains(const String& Substring, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API bool Contains(const char* Substring, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API const char* Find(const String& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API const char* Find(const char* Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API List<const char*> FindAll(const String& Substring, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API List<const char*> FindAll(const char* Substring, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API const char* Split(const String& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API const char* Split(const char* Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API List<const char*> SplitAll(const String& Substring, SearchMode Mode = SearchMode::Substring) const;
		NEXUS_ENGINE_API List<const char*> SplitAll(const char* Substring, SearchMode Mode = SearchMode::Substring) const;

		NEXUS_ENGINE_API uint64 IsEmpty() const { return Count == 0; }
		NEXUS_ENGINE_API uint64 IsNullTerminated() const { return Data[Capacity - 1] == NullChar; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Count; }
		NEXUS_ENGINE_API uint64 GetCapacity() const { return Capacity; }
		NEXUS_ENGINE_API const char* C() const { return Data; }

		template<typename... Args>
		static String Format(const char* Text, Args&&... args);
		template<typename... Args>
		static String Format(uint64 Size, const char* Text, Args&&... args);

		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 Length(const char* Text) { return strlen(Text); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static int8 Compare(const char* Text1, const char* Text2) { return strcmp(Text1, Text2); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Copy(const char* Source, char* Destination, uint64 Capacity) { strcpy_s(Destination, Capacity, Source); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size) { strncpy_s(Destination, Capacity, Source, Size); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Concat(const char* Source, char* Destination, uint64 Capacity) { strcat_s(Destination, Capacity, Source); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size) { strncat_s(Destination, Capacity, Source, Size); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static const char* SearchStr(const char* Source, const char* Substring) { return strstr(Source, Substring); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static const char* SearchChr(const char* Source, const char* Substring) { return strpbrk(Source, Substring); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 Frmt(uint64 Capacity, char* Text, const char* Format, ...) { uint64 Size = 0; NEXUS_VA(Format, Size = vsnprintf(Text, Capacity, Format, ArgList)); return Size; }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Scan(const char* Text, const char* Format, ...) { NEXUS_VA(Format, vsscanf(Text, Format, ArgList)); }
#pragma warning(suppress : 4996)
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe) { if (NotSafe) strncpy(Destination, Source, Size); else Copy(Source, Destination, Capacity, Size); }

		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static int64 ToInteger(const char* Text, int32 Radix = 10) { char* Ptr; return strtol(Text, &Ptr, Radix); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToUnsignedInteger(const char* Text, int32 Radix = 10) { char* Ptr; return strtoul(Text, &Ptr, Radix); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static double ToDouble(const char* Text) { char* Ptr; return strtod(Text, &Ptr); }

		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static String ToStringI(int64 Number, const char* Frmt = "%d") { return Format(Frmt, Number); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static String ToStringU(uint64 Number, const char* Frmt = "%d") { return Format(Frmt, Number); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static String ToStringF(float Number, const char* Frmt = "%.2f") { return Format(Frmt, Number); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static String ToStringD(double Number, const char* Frmt = "%.2f") { return Format(Frmt, Number); }
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static String ToStringB(bool State, const char* Frmt = "%s") { return Format(Frmt, State ? "True" : "False"); }

		static const char NullChar = '\0';

	private:
		enum class SearchBehaviour
		{
			Contains, Find, Split
		};

		void Allocate(uint64 Bytes, uint64 Size, const char* Text);
		void Reallocate(uint64 Bytes);
		void Free();
		void Append(const char* Text, uint64 Size);
		void Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		void Insert(const char* ReferenceText, uint64 ReferenceSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		void Remove(const char* Text, uint64 Size, uint64 Offset, uint64 Occurrence, bool All);
		void ValidateCapacityCount(uint64 Bytes, uint64 Size);
		void ValidateNullTermination();
		void Resize(uint64 Size);
		const char* Search(const char* Substring, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<const char*>* Results) const;

		static const uint8 GuessedFormatingSize = 8;

		Allocator* Allocator;
		uint64 Capacity;
		uint64 Count;
		char* Data;
	};

	template<typename ...Args>
	inline String String::Format(const char* Text, Args&& ...args)
	{
		String Result = String(Length(Text) + sizeof...(args) * GuessedFormatingSize);
		Result.Count = Frmt(Result.Capacity, Result.Data, Text, args...);
		if (Result.Count >= Result.Capacity)
		{
			Result.Grow(Result.Count);
			Result.Count = Frmt(Result.Capacity, Result.Data, Text, args...);
		}
		return Result;
	}

	template<typename ...Args>
	inline String String::Format(uint64 Size, const char* Text, Args&& ...args)
	{
		String Result = String(Size);
		Result.Count = Frmt(Result.Capacity, Result.Data, Text, args...);
		return Result;
	}

	NEXUS_ENGINE_API bool operator==(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator==(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API bool operator==(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator!=(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator!=(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API bool operator!=(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator>(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator>(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API bool operator>(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator>=(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator>=(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API bool operator>=(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator<(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator<(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API bool operator<(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator<=(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API bool operator<=(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API bool operator<=(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API String operator+(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API String operator+(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API String operator+(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API String operator-(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API String operator-(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API String operator-(const char* TextA, const String& TextB);
}
