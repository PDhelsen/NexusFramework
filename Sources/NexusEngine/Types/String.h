#pragma once

#include "Core/NexusEngineCore.h"
#include "Types/Integer.h"
#include "Types/Containers/List.h"
#include "Memory/Allocator/Allocator.h"
#include "Debug/Assert.h"
#include "External/StandardLibrary.h"

namespace NxEn
{
	// TODO: Optimization - Copy / Move - Operator + Self assignement check

	class String;

	//-----------------------------------------------------------------------------------------------------------------------
	// String Functions
	//-----------------------------------------------------------------------------------------------------------------------

	class StringCApi
	{
	public:
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe);

		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 Length(const char* Text);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static int8 Compare(const char* Text1, const char* Text2);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static int8 Compare(const char* Text1, const char* Text2, uint64 Size);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Copy(const char* Source, char* Destination, uint64 Capacity);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Concat(const char* Source, char* Destination, uint64 Capacity);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static const char* SearchStr(const char* Source, const char* Substring);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static const char* SearchChr(const char* Source, const char* Substring);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 Format(uint64 Capacity, char* Text, const char* Format, ...);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static void Scan(const char* Text, const char* Format, ...);

		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static  int64 ToInteger(const char* Text, int32 Radix = 10);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToUnsignedInteger(const char* Text, int32 Radix = 10);
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static double ToDouble(const char* Text);

		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToStringI(int64 Number, uint64 Capacity, char* Text, const char* Frmt = "%d");
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToStringU(uint64 Number, uint64 Capacity, char* Text, const char* Frmt = "%d");
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToStringF(float Number, uint64 Capacity, char* Text, const char* Frmt = "%.2f");
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToStringD(double Number, uint64 Capacity, char* Text, const char* Frmt = "%.2f");
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE static uint64 ToStringB(bool State, uint64 Capacity, char* Text, const char* Frmt = "%s");

		NEXUS_ENGINE_API static const char NullChar = '\0';
	};

	class StringUtility
	{
	public:
		enum class SearchMode
		{
			Substring, Characters
		};

		template<typename... Args>
		static String Format(const char* Text, Args&&... args)
		{
			String Result = String(StringCApi::Length(Text) + sizeof...(args) * GuessFormatingSize);
			Result.Count = StringCApi::Format(Result.GetCapacity(), Result.GetData(), Text, args...);
			if (Result.GetCount() >= Result.GetCapacity())
			{
				Result.Grow(Result.GetCount());
				Result.Count = StringCApi::Format(Result.GetCapacity(), Result.GetData(), Text, args...);
			}
			return Result;
		}

		template<typename... Args>
		static String Format(uint64 Size, const char* Text, Args&&... args)
		{
			String Result = String(Size);
			Result.Count = StringCApi::Format(Result.GetCapacity(), Result.GetData(), Text, args...);
			return Result;
		}

		template<typename... Args>
		static void Scan(const char* Text, Args&&... args)
		{
			StringCApi::Scan(Text, args...);
		}

		NEXUS_ENGINE_API static bool Start(const String& Text, const String& Substring);
		NEXUS_ENGINE_API static bool Start(const String& Text, const char* Substring);
		NEXUS_ENGINE_API static bool Start(const char* Text, const String& Substring);
		NEXUS_ENGINE_API static bool Start(const char* Text, const char* Substring);
		NEXUS_ENGINE_API static bool End(const String& Text, const String& Substring);
		NEXUS_ENGINE_API static bool End(const String& Text, const char* Substring);
		NEXUS_ENGINE_API static bool End(const char* Text, const String& Substring);
		NEXUS_ENGINE_API static bool End(const char* Text, const char* Substring);
		NEXUS_ENGINE_API static bool Contains(const String& Text, const String& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static bool Contains(const String& Text, const char* Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static bool Contains(const char* Text, const String& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static bool Contains(const char* Text, const char* Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Find(const String& Text, const String& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Find(const String& Text, const char* Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Find(const char* Text, const String& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Find(const char* Text, const char* Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> FindAll(const String& Text, const String& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> FindAll(const String& Text, const char* Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> FindAll(const char* Text, const String& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> FindAll(const char* Text, const char* Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Split(const String& Text, const String& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Split(const String& Text, const char* Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Split(const char* Text, const String& Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static const char* Split(const char* Text, const char* Substring, uint64 Offset = 0, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> SplitAll(const String& Text, const String& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> SplitAll(const String& Text, const char* Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> SplitAll(const char* Text, const String& Substring, SearchMode Mode = SearchMode::Substring);
		NEXUS_ENGINE_API static List<const char*> SplitAll(const char* Text, const char* Substring, SearchMode Mode = SearchMode::Substring);

		NEXUS_ENGINE_API static  int64 ToInteger(const String& Text, int32 Radix = 10);
		NEXUS_ENGINE_API static  int64 ToInteger(const char* Text, int32 Radix = 10);
		NEXUS_ENGINE_API static uint64 ToUnsignedInteger(const String& Text, int32 Radix = 10);
		NEXUS_ENGINE_API static uint64 ToUnsignedInteger(const char* Text, int32 Radix = 10);
		NEXUS_ENGINE_API static double ToDouble(const String& Text);
		NEXUS_ENGINE_API static double ToDouble(const char* Text);

		NEXUS_ENGINE_API static String ToStringI(int64 Number, const char* Frmt = "%d");
		NEXUS_ENGINE_API static String ToStringU(uint64 Number, const char* Frmt = "%d");
		NEXUS_ENGINE_API static String ToStringF(float Number, const char* Frmt = "%.2f");
		NEXUS_ENGINE_API static String ToStringD(double Number, const char* Frmt = "%.2f");
		NEXUS_ENGINE_API static String ToStringB(bool State, const char* Frmt = "%s");

		NEXUS_ENGINE_API static const char NullChar = StringCApi::NullChar;

	private:
		enum class SearchBehaviour
		{
			Contains, Find, Split
		};

		static const char* Search(const char* Text, const char* Substring, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<const char*>* Results);

		static const uint8 GuessFormatingSize = 8;
	};

	NEXUS_ENGINE_API String operator+(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API String operator+(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API String operator+(const char* TextA, const String& TextB);
	NEXUS_ENGINE_API String operator-(const String& TextA, const String& TextB);
	NEXUS_ENGINE_API String operator-(const String& TextA, const char* TextB);
	NEXUS_ENGINE_API String operator-(const char* TextA, const String& TextB);
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

	//-----------------------------------------------------------------------------------------------------------------------
	// String Data
	//-----------------------------------------------------------------------------------------------------------------------

	class String
	{
		friend StringUtility;

	public:
		NEXUS_ENGINE_API String();
		NEXUS_ENGINE_API String(uint64 Bytes);
		NEXUS_ENGINE_API String(const char* Text);
		NEXUS_ENGINE_API String(const char* Text, uint64 Size);
		NEXUS_ENGINE_API String(const String& Other);
		NEXUS_ENGINE_API String(String&& Other) noexcept;
		NEXUS_ENGINE_API ~String();

		NEXUS_ENGINE_API String& operator=(const String& Other);
		NEXUS_ENGINE_API String& operator+=(const String& Other);
		NEXUS_ENGINE_API String& operator+=(const char* Other);
		NEXUS_ENGINE_API String& operator-=(const String& Other);
		NEXUS_ENGINE_API String& operator-=(const char* Other);

		NEXUS_ENGINE_API String& Append(const String& Text);
		NEXUS_ENGINE_API String& Append(const char* Text);
		NEXUS_ENGINE_API String& Replace(const String& Old, const String& New);
		NEXUS_ENGINE_API String& Replace(const String& Old, const char* New);
		NEXUS_ENGINE_API String& Replace(const char* Old, const String& New);
		NEXUS_ENGINE_API String& Replace(const char* Old, const char* New);
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

		NEXUS_ENGINE_API const char* C() const { return GetBuffer(); }
		NEXUS_ENGINE_API uint64 IsEmpty() const { return Count == 0; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Count; }
		NEXUS_ENGINE_API uint64 GetCapacity() const { return Capacity; }

		NEXUS_ENGINE_API static String Empty;
		NEXUS_ENGINE_API static const uint8 SmallStringCapacity = 16;

	private:
		void Allocate(Allocator* Al, uint64 Bytes, uint64 Size, const char* Text);
		void Reallocate(uint64 Bytes);
		void Free();
		void Resize(uint64 Size);
		void ValidateCapacityCount(uint64 Bytes, uint64 Size);
		void ValidateNullTermination();
		void Append(const char* Text, uint64 Size);
		void Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		void Insert(const char* ReferenceText, uint64 ReferenceSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		void Remove(const char* Text, uint64 Size, uint64 Offset, uint64 Occurrence, bool All);

		NEXUS_FORCE_INLINE const char* GetBuffer() const { return Sso() ? Data.Small : Data.Large; }
		NEXUS_FORCE_INLINE char* GetData() { return Sso() ? Data.Small : Data.Large; }
		NEXUS_FORCE_INLINE bool Sso() const { return Capacity <= SmallStringCapacity; }

		union Buffer
		{
			char* Large;
			char Small[SmallStringCapacity];
		};

		Allocator* Allctr;
		uint64 Capacity;
		uint64 Count;
		Buffer Data;
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
}
