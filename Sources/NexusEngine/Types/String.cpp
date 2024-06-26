#include "Core/NexusEnginePch.h"
#include "String.h"

namespace NxEn
{
	//-----------------------------------------------------------------------------------------------------------------------
	// String C Api
	//-----------------------------------------------------------------------------------------------------------------------

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size, bool NotSafe)
	{
		if (NotSafe)
		{
#pragma warning(suppress : 4996)
			strncpy(Destination, Source, Size);
		}
		else
		{
			StringCApi::Copy(Source, Destination, Capacity, Size);
		}
	}

	uint64 StringCApi::Length(const char* Text)
	{
		return strlen(Text);
	}

	int8 StringCApi::Compare(const char* Text1, const char* Text2)
	{
		return strcmp(Text1, Text2);
	}

	int8 StringCApi::Compare(const char* Text1, const char* Text2, uint64 Size)
	{
		return strncmp(Text1, Text2, Size);
	}

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity)
	{
		strcpy_s(Destination, Capacity, Source);
	}

	void StringCApi::Copy(const char* Source, char* Destination, uint64 Capacity, uint64 Size)
	{
		strncpy_s(Destination, Capacity, Source, Size);
	}

	void StringCApi::Concat(const char* Source, char* Destination, uint64 Capacity)
	{
		strcat_s(Destination, Capacity, Source);
	}

	void StringCApi::Concat(const char* Source, char* Destination, uint64 Capacity, uint64 Size)
	{
		strncat_s(Destination, Capacity, Source, Size);
	}

	const char* StringCApi::SearchStr(const char* Source, const char* Substring)
	{
		return strstr(Source, Substring);
	}

	const char* StringCApi::SearchChr(const char* Source, const char* Substring)
	{
		return strpbrk(Source, Substring);
	}

	uint64 StringCApi::Format(uint64 Capacity, char* Text, const char* Format, ...)
	{
		uint64 Size = 0;
		NEXUS_VA(Format, Size = vsnprintf(Text, Capacity, Format, ArgList));
		return Size;
	}

	void StringCApi::Scan(const char* Text, const char* Format, ...)
	{
		NEXUS_VA(Format, vsscanf(Text, Format, ArgList));
	}

	int64 StringCApi::ToInteger(const char* Text, int32 Radix)
	{
		char* Ptr; return strtol(Text, &Ptr, Radix);
	}

	uint64 StringCApi::ToUnsignedInteger(const char* Text, int32 Radix)
	{
		char* Ptr; return strtoul(Text, &Ptr, Radix);
	}

	double StringCApi::ToDouble(const char* Text)
	{
		char* Ptr; return strtod(Text, &Ptr);
	}

	uint64 StringCApi::ToStringI(int64 Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringU(uint64 Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringF(float Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringD(double Number, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, Number);
	}

	uint64 StringCApi::ToStringB(bool State, uint64 Capacity, char* Text, const char* Frmt)
	{
		return StringCApi::Format(Capacity, Text, Frmt, State ? "True" : "False");
	}

	//-----------------------------------------------------------------------------------------------------------------------
	// String Utility
	//-----------------------------------------------------------------------------------------------------------------------

	bool StringUtility::Start(const String& Text, const String& Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring.C());
		return Result && Text.C() == Result;
	}

	bool StringUtility::Start(const String& Text, const char* Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring);
		return Result && Text.C() == Result;
	}

	bool StringUtility::Start(const char* Text, const String& Substring)
	{
		const char* Result = StringCApi::SearchStr(Text, Substring.C());
		return Result && Text == Result;
	}

	bool StringUtility::Start(const char* Text, const char* Substring)
	{
		const char* Result = StringCApi::SearchStr(Text, Substring);
		return Result && Text == Result;
	}

	bool StringUtility::End(const String& Text, const String& Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring.C());
		return Result && Substring.GetCount() == StringCApi::Length(Result);
	}

	bool StringUtility::End(const String& Text, const char* Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring);
		return Result && StringCApi::Length(Substring) == StringCApi::Length(Result);
	}

	bool StringUtility::End(const char* Text, const String& Substring)
	{
		const char* Result = StringCApi::SearchStr(Text, Substring.C());
		return Result && Substring.GetCount() == StringCApi::Length(Result);
	}

	bool StringUtility::End(const char* Text, const char* Substring)
	{
		const char* Result = StringCApi::SearchStr(Text, Substring);
		return Result && StringCApi::Length(Substring) == StringCApi::Length(Result);
	}

	bool StringUtility::Contains(const String& Text, const String& Substring, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool StringUtility::Contains(const String& Text, const char* Substring, SearchMode Mode)
	{
		return Search(Text.C(), Substring, SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool StringUtility::Contains(const char* Text, const String& Substring, SearchMode Mode)
	{
		return Search(Text, Substring.C(), SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool StringUtility::Contains(const char* Text, const char* Substring, SearchMode Mode)
	{
		return Search(Text, Substring, SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	const char* StringUtility::Find(const String& Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* StringUtility::Find(const String& Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring, SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* StringUtility::Find(const char* Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring.C(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* StringUtility::Find(const char* Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring, SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<const char*> StringUtility::FindAll(const String& Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.C(), Substring.C(), SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> StringUtility::FindAll(const String& Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.C(), Substring, SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> StringUtility::FindAll(const char* Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring.C(), SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> StringUtility::FindAll(const char* Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring, SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	const char* StringUtility::Split(const String& Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* StringUtility::Split(const String& Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring, SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* StringUtility::Split(const char* Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring.C(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* StringUtility::Split(const char* Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring, SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<const char*> StringUtility::SplitAll(const String& Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.C(), Substring.C(), SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> StringUtility::SplitAll(const String& Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.C(), Substring, SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> StringUtility::SplitAll(const char* Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring.C(), SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> StringUtility::SplitAll(const char* Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring, SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	int64 StringUtility::ToInteger(const String& Text, int32 Radix)
	{
		return StringCApi::ToInteger(Text.C());
	}

	int64 StringUtility::ToInteger(const char* Text, int32 Radix)
	{
		return StringCApi::ToInteger(Text);
	}

	uint64 StringUtility::ToUnsignedInteger(const String& Text, int32 Radix)
	{
		return StringCApi::ToUnsignedInteger(Text.C());
	}

	uint64 StringUtility::ToUnsignedInteger(const char* Text, int32 Radix)
	{
		return StringCApi::ToUnsignedInteger(Text);
	}

	double StringUtility::ToDouble(const String& Text)
	{
		return StringCApi::ToDouble(Text.C());
	}

	double StringUtility::ToDouble(const char* Text)
	{
		return StringCApi::ToDouble(Text);
	}

	String StringUtility::ToStringI(int64 Number, const char* Frmt)
	{
		String Result = String();
		StringCApi::ToStringI(Number, Result.GetCapacity(), Result.GetData(), Frmt);
		return Move(Result);
	}

	String StringUtility::ToStringU(uint64 Number, const char* Frmt)
	{
		String Result = String();
		StringCApi::ToStringU(Number, Result.GetCapacity(), Result.GetData(), Frmt);
		return Move(Result);
	}

	String StringUtility::ToStringF(float Number, const char* Frmt)
	{
		String Result = String();
		StringCApi::ToStringF(Number, Result.GetCapacity(), Result.GetData(), Frmt);
		return Move(Result);
	}

	String StringUtility::ToStringD(double Number, const char* Frmt)
	{
		String Result = String();
		StringCApi::ToStringD(Number, Result.GetCapacity(), Result.GetData(), Frmt);
		return Move(Result);
	}

	String StringUtility::ToStringB(bool State, const char* Frmt)
	{
		String Result = String();
		StringCApi::ToStringB(State, Result.GetCapacity(), Result.GetData(), Frmt);
		return Move(Result);
	}

	const char* StringUtility::Search(const char* Text, const char* Substring, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<const char*>* Results)
	{
		uint64 Index = 0;
		const char* Previous = Text;
		const char* Pointer = Text;
		const char* Result = nullptr;

		do
		{
			switch (Mode)
			{
			case SearchMode::Substring: Pointer = StringCApi::SearchStr(Pointer, Substring); break;
			case SearchMode::Characters: Pointer = StringCApi::SearchChr(Pointer, Substring); break;
			}

			switch (Behaviour)
			{
			case SearchBehaviour::Contains: Result = Pointer; break;
			case SearchBehaviour::Find: Result = Pointer; break;
			case SearchBehaviour::Split: Result = Previous; break;
			}

			if (!Pointer)
			{
				break;
			}

			if (Results)
			{
				Results->Append(Result);
			}
			else if (Index == Offset)
			{
				break;
			}

			Index++;
			Pointer++;
			Previous = Pointer;
		} while (true);

		if (Results && Behaviour == SearchBehaviour::Split)
		{
			Results->Append(Result);
		}

		return Result;
	}

	//-----------------------------------------------------------------------------------------------------------------------
	// Operator
	//-----------------------------------------------------------------------------------------------------------------------

	String operator+(const String& TextA, const String& TextB)
	{
		String Return = String(TextA.GetCount() + TextB.GetCount());
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator+(const String& TextA, const char* TextB)
	{
		String Return = String(TextA.GetCount() + StringCApi::Length(TextB));
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator+(const char* TextA, const String& TextB)
	{
		String Return = String(StringCApi::Length(TextA) + TextB.GetCount());
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator-(const String& TextA, const String& TextB)
	{
		String Return = String(TextA.GetCount());
		Return.Append(TextA);
		Return.Remove(TextB, 0, 0, true);
		return Move(Return);
	}

	String operator-(const String& TextA, const char* TextB)
	{
		String Return = String(TextA.GetCount());
		Return.Append(TextA);
		Return.Remove(TextB, 0, 0, true);
		return Move(Return);
	}

	String operator-(const char* TextA, const String& TextB)
	{
		String Return = String(StringCApi::Length(TextA));
		Return.Append(TextA);
		Return.Remove(TextB, 0, 0, true);
		return Move(Return);
	}

	bool operator==(const String& TextA, const String& TextB)
	{
		return TextA.GetCount() == TextB.GetCount() && StringCApi::Compare(TextA.C(), TextB.C()) == 0;
	}

	bool operator==(const String& TextA, const char* TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB) == 0;
	}

	bool operator==(const char* TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA, TextB.C()) == 0;
	}

	bool operator!=(const String& TextA, const String& TextB)
	{
		return !(TextA == TextB);
	}

	bool operator!=(const String& TextA, const char* TextB)
	{
		return !(TextA == TextB);
	}

	bool operator!=(const char* TextA, const String& TextB)
	{
		return !(TextA == TextB);
	}

	bool operator>(const String& TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB.C()) > 0;
	}

	bool operator>(const String& TextA, const char* TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB) > 0;
	}

	bool operator>(const char* TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA, TextB.C()) > 0;
	}

	bool operator>=(const String& TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB.C()) >= 0;
	}

	bool operator>=(const String& TextA, const char* TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB) >= 0;
	}

	bool operator>=(const char* TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA, TextB.C()) >= 0;
	}

	bool operator<(const String& TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB.C()) < 0;
	}

	bool operator<(const String& TextA, const char* TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB) < 0;
	}

	bool operator<(const char* TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA, TextB.C()) < 0;
	}

	bool operator<=(const String& TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB.C()) <= 0;
	}

	bool operator<=(const String& TextA, const char* TextB)
	{
		return StringCApi::Compare(TextA.C(), TextB) <= 0;
	}

	bool operator<=(const char* TextA, const String& TextB)
	{
		return StringCApi::Compare(TextA, TextB.C()) <= 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------
	// String
	//-----------------------------------------------------------------------------------------------------------------------

	String String::Empty = String();

	String::String()
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Memory::GetActiveAllocator(), SmallStringCapacity, 0, nullptr);
	}

	String::String(uint64 Bytes)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Memory::GetActiveAllocator(), Bytes, 0, nullptr);
	}

	String::String(const char* Text)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		uint64 Size = StringCApi::Length(Text);
		Allocate(Memory::GetActiveAllocator(), Size, Size, Text);
	}

	String::String(const char* Text, uint64 Size)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Memory::GetActiveAllocator(), Size, Size, Text);
	}

	String::String(const String& Other)
		: Allctr(Other.Allctr), Capacity(Other.Capacity), Count(Other.Count)
	{
		if (Other.Sso())
		{
			StringCApi::Copy(Other.Data.Small, Data.Small, SmallStringCapacity);
		}
		else
		{
			Data.Large = Other.Data.Large;
		}
	}

	String::String(String&& Other) noexcept
		: Allctr(Other.Allctr), Capacity(Other.Capacity), Count(Other.Count)
	{
		if (Other.Sso())
		{
			StringCApi::Copy(Other.Data.Small, Data.Small, SmallStringCapacity);
		}
		else
		{
			Data.Large = Other.Data.Large;
		}

		Other.Allctr = nullptr;
		Other.Capacity = 0;
		Other.Count = 0;
		Other.Data.Large = nullptr;
	}

	String::~String()
	{
		Free();
	}

	String& String::operator=(const String& Other)
	{
		Resize(Other.Count);
		StringCApi::Copy(Other.GetBuffer(), GetData(), Capacity);
		return *this;
	}

	String& String::operator+=(const String& Other)
	{
		Append(Other);
		return *this;
	}

	String& String::operator+=(const char* Other)
	{
		Append(Other);
		return *this;
	}

	String& String::operator-=(const String& Other)
	{
		Remove(Other, 0, 0, true);
		return *this;
	}

	String& String::operator-=(const char* Other)
	{
		Remove(Other, 0, 0, true);
		return *this;
	}

	String& String::Append(const String& Text)
	{
		Append(Text.GetBuffer(), Text.Count);
		return *this;
	}

	String& String::Append(const char* Text)
	{
		Append(Text, StringCApi::Length(Text));
		return *this;
	}

	String& String::Replace(const String& Old, const String& New)
	{
		return Assign(Old, New, 0, 1, true);
	}

	String& String::Replace(const String& Old, const char* New)
	{
		return Assign(Old, New, 0, 1, true);
	}

	String& String::Replace(const char* Old, const String& New)
	{
		return Assign(Old, New, 0, 1, true);
	}

	String& String::Replace(const char* Old, const char* New)
	{
		return Assign(Old, New, 0, 1, true);
	}

	String& String::Assign(const String& OldText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText.GetBuffer(), OldText.Count, NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const String& OldText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText.GetBuffer(), OldText.Count, NewText, StringCApi::Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const char* OldText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText, StringCApi::Length(OldText), NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const char* OldText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText, StringCApi::Length(OldText), NewText, StringCApi::Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const String& ReferenceText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.GetBuffer(), ReferenceText.Count, NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const String& ReferenceText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.GetBuffer(), ReferenceText.Count, NewText, StringCApi::Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const char* ReferenceText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText, StringCApi::Length(ReferenceText), NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const char* ReferenceText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText, StringCApi::Length(ReferenceText), NewText, StringCApi::Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Remove(const String& Text, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Remove(Text.GetBuffer(), Text.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Remove(const char* Text, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Remove(Text, StringCApi::Length(Text), Offset, Occurrence, All);
		return *this;
	}

	String& String::Clear()
	{
		Resize(0);
		return *this;
	}

	void String::Grow(uint64 Size)
	{
		if (Size <= Capacity)
		{
			return;
		}

		Reallocate(Size);
	}

	void String::Shrink(uint64 Size /*= 0*/)
	{
		if (Size >= Capacity)
		{
			return;
		}

		Reallocate(Size);
	}

	void String::Allocate(Allocator* Al, uint64 Bytes, uint64 Size, const char* Text)
	{
		ValidateCapacityCount(Bytes, Size);

		Allctr = Al;
		if (!Sso())
		{
			Data.Large = (char*)Memory::Allocate(Capacity, NEXUS_MEMORY_ALIGN, Allctr);
		}

		if (Text)
		{
			Memory::MemCopy(Text, GetData(), Capacity);
		}

		ValidateNullTermination();
	}

	void String::Reallocate(uint64 Bytes)
	{
		bool WasSso = Sso();

		ValidateCapacityCount(Bytes, Count);

		if (!Sso() && !WasSso)
		{
			Data.Large = (char*)Memory::Realloc(Data.Large, Capacity, NEXUS_MEMORY_ALIGN, Allctr);
		}
		else if (!Sso() && WasSso)
		{
			char Temp[SmallStringCapacity];
			StringCApi::Copy(Data.Small, Temp, SmallStringCapacity);

			Data.Large = (char*)Memory::Allocate(Capacity, NEXUS_MEMORY_ALIGN, Allctr);
			StringCApi::Copy(Temp, Data.Large, Capacity);
		}
		else
		{
			// Once grow over the Sso limit, it cannot go back to Sso.
			// The cost of allocating has been paid, so there is no point to freeing the memory until the string is destroyed
			NEXUS_ASSERT(false, "Not supposed to reallocate from sso to sso");
		}

		ValidateNullTermination();
	}

	void String::Free()
	{
		if (!Sso())
		{
			Memory::Free(Data.Large, Allctr);
		}
	}

	void String::Resize(uint64 Size)
	{
		Count = Size;

		if (Count >= Capacity)
		{
			Reallocate(Count);
		}

		ValidateNullTermination();
	}

	void String::ValidateCapacityCount(uint64 Bytes, uint64 Size)
	{
		Count = Size;

		// Once the string exceed the Sso limit, it cannot go back under the Sso limit.
		uint64 SmallString = !Sso() ? SmallStringCapacity + 1 : SmallStringCapacity;
		uint64 SmallestCapacity = Count + 1 >= SmallString ? Count + 1 : SmallString;
		Capacity = Bytes >= SmallestCapacity ? Bytes : SmallestCapacity;
	}

	void String::ValidateNullTermination()
	{
		GetData()[Count] = StringCApi::NullChar;
		GetData()[Capacity - 1] = StringCApi::NullChar;
	}

	void String::Append(const char* Text, uint64 Size)
	{
		Resize(Count + Size);
		StringCApi::Concat(Text, GetData(), Capacity);
	}

	void String::Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset, uint64 Occurrence, bool All)
	{
		if (OldSize <= 0 || NewSize <= 0 || StringCApi::Compare(OldText, NewText) == 0)
		{
			return;
		}

		uint64 Index = 0;
		uint64 Modified = 0;
		char* Substring = GetData();
		int64 SizeDiff = NewSize - OldSize;

		do
		{
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, OldText));

			if (Substring)
			{
				if (Index >= Offset)
				{
					if (SizeDiff > 0)
					{
						Resize(Count + SizeDiff);
						uint64 Remaining = StringCApi::Length(Substring + OldSize);
						if (Remaining > 0)
						{
							Memory::MemCopy(Substring + OldSize, Substring + NewSize, Remaining);
						}
					}
					else if (SizeDiff < 0)
					{
						uint64 Remaining = StringCApi::Length(Substring + OldSize);
						if (Remaining > 0)
						{
							Memory::MemCopy(Substring + OldSize, Substring + NewSize, Remaining);
						}
						Resize(Count + SizeDiff);
					}

					StringCApi::Copy(NewText, Substring, Capacity, NewSize, true);

					Modified++;
				}
				else
				{
					Substring++;
				}
			}

			Index++;
		} while (Substring && (All || (!All && Modified < Occurrence)));
	}

	void String::Insert(const char* ReferenceText, uint64 ReferenceSize, const char* NewText, uint64 NewSize, uint64 Offset, uint64 Occurrence, bool All)
	{
		if (ReferenceSize <= 0 || NewSize <= 0)
		{
			return;
		}

		uint64 Index = 0;
		uint64 Modified = 0;
		char* Substring = GetData();

		do
		{
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, ReferenceText));

			if (Substring)
			{
				Substring += ReferenceSize;

				if (Index >= Offset)
				{
					Resize(Count + NewSize);
					uint64 Remaining = StringCApi::Length(Substring);
					if (Remaining > 0)
					{
						Memory::MemCopy(Substring, Substring + NewSize, StringCApi::Length(Substring));
					}
					StringCApi::Copy(NewText, Substring, Capacity, NewSize, true);

					Modified++;
				}
			}

			Index++;
		} while (Substring && (All || (!All && Modified < Occurrence)));
	}

	void String::Remove(const char* Text, uint64 Size, uint64 Offset, uint64 Occurrence, bool All)
	{
		if (Size <= 0)
		{
			return;
		}

		uint64 Index = 0;
		uint64 Removed = 0;
		char* Substring = GetData();

		do
		{
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, Text));

			if (Substring)
			{
				if (Index >= Offset)
				{
					uint64 Remaining = StringCApi::Length(Substring) - Size;
					if (Remaining > 0)
					{
						Memory::MemCopy(Substring + Size, Substring, Remaining);
					}

					Resize(Count - Size);
					Removed++;
				}
				else
				{
					Substring++;
				}
			}

			Index++;
		} while (Substring && (All || (!All && Removed < Occurrence)));
	}
}
