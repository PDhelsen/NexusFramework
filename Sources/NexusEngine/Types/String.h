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
	struct StringView;
	struct StringId;

	//-----------------------------------------------------------------------------------------------------------------------
	// String Data
	//-----------------------------------------------------------------------------------------------------------------------

	class String
	{
		friend class StringUtility;

	public:
		NEXUS_ENGINE_API String();
		NEXUS_ENGINE_API String(uint64 Bytes);
		NEXUS_ENGINE_API String(const char* Text);
		NEXUS_ENGINE_API String(const char* Text, uint64 Size);
		NEXUS_ENGINE_API String(const StringView& Text);
		NEXUS_ENGINE_API String(const String& Other);
		NEXUS_ENGINE_API String(String&& Other) noexcept;
		NEXUS_ENGINE_API ~String();

		NEXUS_ENGINE_API String& operator=(const String& Other);
		NEXUS_ENGINE_API String& operator+=(const StringView& Other);
		NEXUS_ENGINE_API String& operator-=(const StringView& Other);

		NEXUS_ENGINE_API String& Append(const StringView& Text);
		NEXUS_ENGINE_API String& Replace(const StringView& Old, const StringView& New);
		NEXUS_ENGINE_API String& Assign(const StringView& OldText, const StringView& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Insert(const StringView& ReferenceText, const StringView& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Remove(const StringView& Text, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Clear();

		NEXUS_ENGINE_API bool Start(const StringView& Substring) const;
		NEXUS_ENGINE_API bool End(const StringView& Substring) const;
		NEXUS_ENGINE_API bool Contains(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Find(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> FindAll(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Split(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> SplitAll(const StringView& Substring) const;

		NEXUS_ENGINE_API void Grow(uint64 Size);
		NEXUS_ENGINE_API void Shrink(uint64 Size = 0);
		NEXUS_ENGINE_API void Validate();

		NEXUS_ENGINE_API StringView ToView() const;
		NEXUS_ENGINE_API StringView ToView(uint64 Offset, uint64 Size) const;

		NEXUS_ENGINE_API const char* C() const { return GetBuffer(); }
		NEXUS_ENGINE_API bool IsEmpty() const { return Count == 0; }
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

	struct StringView
	{
	public:
		NEXUS_ENGINE_API StringView(const char* Text);
		NEXUS_ENGINE_API StringView(const char* Text, uint64 Size);
		NEXUS_ENGINE_API StringView(const String& Text);

		NEXUS_ENGINE_API bool Start(const StringView& Substring) const;
		NEXUS_ENGINE_API bool End(const StringView& Substring) const;
		NEXUS_ENGINE_API bool Contains(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Find(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> FindAll(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Split(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> SplitAll(const StringView& Substring) const;

		NEXUS_ENGINE_API String ToString() const;
		NEXUS_ENGINE_API StringView ToView(uint64 Offset, uint64 Size) const;

		NEXUS_ENGINE_API const char* C() const { return Data; }
		NEXUS_ENGINE_API bool IsEmpty() const { return Count == 0; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Count; }

	private:
		const char* Data;
		uint64 Count;
	};

	struct StringId
	{
	public:
		NEXUS_ENGINE_API explicit StringId(const StringView& Text);

		NEXUS_ENGINE_API bool operator==(const StringId& Other) const;
		NEXUS_ENGINE_API bool operator==(GUID Other) const;
		NEXUS_ENGINE_API bool operator!=(const StringId& Other) const;
		NEXUS_ENGINE_API bool operator!=(GUID Other) const;
		NEXUS_ENGINE_API operator GUID() const;

		NEXUS_ENGINE_API const String& ToString() const { return Tables[Id]; };
		NEXUS_ENGINE_API const GUID GetId() const { return Id; };

	private:
		static GUID InternString(const StringView& Text);

		static Dictionary<GUID, String> Tables;

		const GUID Id;
	};

	NEXUS_ENGINE_API StringId operator""_Sid(const char* Text, uint64 Size);

	//-----------------------------------------------------------------------------------------------------------------------
	// String Functions
	//-----------------------------------------------------------------------------------------------------------------------

	class StringCApi
	{
	public:
		NEXUS_ENGINE_API static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe);
		NEXUS_ENGINE_API static const char* SearchStr(const char* Source, const char* Substring, uint64 Size);

		NEXUS_ENGINE_API static uint64 Length(const char* Text);
		NEXUS_ENGINE_API static int8 Compare(const char* Text1, const char* Text2);
		NEXUS_ENGINE_API static int8 Compare(const char* Text1, const char* Text2, uint64 Size);
		NEXUS_ENGINE_API static void Copy(const char* Source, char* Destination, uint64 Capacity);
		NEXUS_ENGINE_API static void Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NEXUS_ENGINE_API static void Concat(const char* Source, char* Destination, uint64 Capacity);
		NEXUS_ENGINE_API static void Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size);
		NEXUS_ENGINE_API static const char* SearchStr(const char* Source, const char* Substring);
		NEXUS_ENGINE_API static const char* SearchChr(const char* Source, const char* Substring);
		NEXUS_ENGINE_API static uint64 Format(uint64 Capacity, char* Text, const char* Format, ...);
		NEXUS_ENGINE_API static void Scan(const char* Text, const char* Format, ...);

		NEXUS_ENGINE_API static  int64 ToInteger(const char* Text, int32 Radix = 10);
		NEXUS_ENGINE_API static uint64 ToUnsignedInteger(const char* Text, int32 Radix = 10);
		NEXUS_ENGINE_API static double ToDouble(const char* Text);

		NEXUS_ENGINE_API static uint64 ToStringI(int64 Number, uint64 Capacity, char* Text, const char* Frmt = "%d");
		NEXUS_ENGINE_API static uint64 ToStringU(uint64 Number, uint64 Capacity, char* Text, const char* Frmt = "%d");
		NEXUS_ENGINE_API static uint64 ToStringF(float Number, uint64 Capacity, char* Text, const char* Frmt = "%.2f");
		NEXUS_ENGINE_API static uint64 ToStringD(double Number, uint64 Capacity, char* Text, const char* Frmt = "%.2f");
		NEXUS_ENGINE_API static uint64 ToStringB(bool State, uint64 Capacity, char* Text, const char* Frmt = "%s");

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
		static String Format(const StringView& Text, Args&&... args)
		{
			String Result = String(Text.GetCount() + sizeof...(args) * GuessFormatingSize);
			uint64 Size = StringCApi::Format(Result.GetCapacity(), Result.GetData(), Text.C(), args...);
			if (Size >= Result.GetCapacity())
			{
				Result.Grow(Size);
				StringCApi::Format(Result.GetCapacity(), Result.GetData(), Text.C(), args...);
			}
			Result.Validate();
			return Move(Result);
		}

		template<typename... Args>
		static String Format(uint64 Size, const StringView& Text, Args&&... args)
		{
			String Result = String(Size);
			StringCApi::Format(Result.GetCapacity(), Result.GetData(), Text.C(), args...);
			Result.Validate();
			return Move(Result);
		}

		template<typename... Args>
		static void Scan(const StringView& Text, Args&&... args)
		{
			StringCApi::Scan(Text.C(), args...);
		}

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

using NxEn::operator""_Sid;
