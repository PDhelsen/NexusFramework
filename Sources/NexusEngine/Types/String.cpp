#include "Core/NexusEnginePch.h"
#include "String.h"

namespace NxEn
{
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

	String::String(const StringView& Text)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		uint64 Size = Text.GetCount();
		Allocate(Memory::GetActiveAllocator(), Size, Size, Text.C());
	}

	String::String(const String& Other)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Other.Allctr, Other.Capacity, Other.Count, Other.GetBuffer());
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

	String& String::operator+=(const StringView& Other)
	{
		Append(Other);
		return *this;
	}

	String& String::operator-=(const StringView& Other)
	{
		Remove(Other, 0, 0, true);
		return *this;
	}

	String& String::Append(const StringView& Text)
	{
		Append(Text.C(), Text.GetCount());
		return *this;
	}

	String& String::Replace(const StringView& Old, const StringView& New)
	{
		return Assign(Old, New, 0, 1, true);
	}

	String& String::Assign(const StringView& OldText, const StringView& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText.C(), OldText.GetCount(), NewText.C(), NewText.GetCount(), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const StringView& ReferenceText, const StringView& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.C(), ReferenceText.GetCount(), NewText.C(), NewText.GetCount(), Offset, Occurrence, All);
		return *this;
	}

	String& String::Remove(const StringView& Text, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Remove(Text.C(), Text.GetCount(), Offset, Occurrence, All);
		return *this;
	}

	String& String::Clear()
	{
		Resize(0);
		return *this;
	}

	bool String::Start(const StringView& Substring) const
	{
		return StringUtility::Start(C(), Substring);
	}

	bool String::End(const StringView& Substring) const
	{
		return StringUtility::End(C(), Substring);
	}

	bool String::Contains(const StringView& Substring) const
	{
		return StringUtility::Contains(C(), Substring);
	}

	StringView String::Find(const StringView& Substring, uint64 Offset) const
	{
		return StringUtility::Find(C(), Substring, Offset);
	}

	List<StringView> String::FindAll(const StringView& Substring) const
	{
		return Move(StringUtility::FindAll(C(), Substring));
	}

	StringView String::Split(const StringView& Substring, uint64 Offset) const
	{
		return StringUtility::Split(C(), Substring, Offset);
	}

	List<StringView> String::SplitAll(const StringView& Substring) const
	{
		return Move(StringUtility::SplitAll(C(), Substring));
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

	void String::Validate()
	{
		Count = StringCApi::Length(C());
		ValidateNullTermination();
	}

	StringView String::ToView() const
	{
		return StringView(*this);
	}

	StringView String::ToView(uint64 Offset, uint64 Size) const
	{
		NEXUS_ASSERT(Offset + Size <= Count, "Invalid String view");
		return StringView(C() + Offset, Size);
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
		StringCApi::Concat(Text, GetData(), Capacity, Size);
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
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, OldText, OldSize));

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
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, ReferenceText, ReferenceSize));

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
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, Text, Size));

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

	//-----------------------------------------------------------------------------------------------------------------------
	// String View
	//-----------------------------------------------------------------------------------------------------------------------

	StringView::StringView(const char* Text)
		:Data(Text), Count(StringCApi::Length(Text))
	{
	}

	StringView::StringView(const char* Text, uint64 Size)
		:Data(Text), Count(Size)
	{
	}

	StringView::StringView(const String& Text)
		:Data(Text.C()), Count(Text.GetCount())
	{
	}

	bool StringView::Start(const StringView& Substring) const
	{
		return StringUtility::Start(C(), Substring);
	}

	bool StringView::End(const StringView& Substring) const
	{
		return StringUtility::End(C(), Substring);
	}

	bool StringView::Contains(const StringView& Substring) const
	{
		return StringUtility::Contains(C(), Substring);
	}

	StringView StringView::Find(const StringView& Substring, uint64 Offset) const
	{
		return StringUtility::Find(C(), Substring, Offset);
	}

	List<StringView> StringView::FindAll(const StringView& Substring) const
	{
		return Move(StringUtility::FindAll(C(), Substring));
	}

	StringView StringView::Split(const StringView& Substring, uint64 Offset) const
	{
		return StringUtility::Split(C(), Substring, Offset);
	}

	List<StringView> StringView::SplitAll(const StringView& Substring) const
	{
		return Move(StringUtility::SplitAll(C(), Substring));
	}

	String StringView::ToString() const
	{
		return Move(String(*this));
	}

	StringView StringView::ToView(uint64 Offset, uint64 Size) const
	{
		NEXUS_ASSERT(Offset + Size <= Count, "Invalid String view");
		return StringView(Data + Offset, Size);
	}

	//-----------------------------------------------------------------------------------------------------------------------
	// String Id
	//-----------------------------------------------------------------------------------------------------------------------

	Dictionary<GUID, String> StringId::Tables = Dictionary<GUID, String>();

	StringId::StringId(const StringView& Text)
		: Id(InternString(Text))
	{
	}

	bool StringId::operator==(const StringId& Other) const
	{
		return Id == Other.Id;
	}

	bool StringId::operator==(GUID Other) const
	{
		return Id == Other;
	}

	bool StringId::operator!=(const StringId& Other) const
	{
		return !(*this == Other);
	}

	bool StringId::operator!=(GUID Other) const
	{
		return !(*this == Other);
	}

	StringId::operator GUID() const
	{
		return Id;
	}

	GUID StringId::InternString(const StringView& Text)
	{
		GUID Id = Hash<StringView>::HashObject(Text);
		if (!Tables.ContainsKey(Id))
		{
			Tables.AppendConstruct(Id, Text);
		}
		return Id;
	}

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

	const char* StringCApi::SearchStr(const char* Source, const char* Substring, uint64 Size)
	{
		uint64 Count = 0;
		bool Finding = false;
		const char* Result = nullptr;
		const char* Current = Source;
		const char* Target = Substring;

		while (*Current != NullChar)
		{
			if (*Current == *Target)
			{
				if (!Finding)
				{
					Count = 0;
					Finding = true;
					Result = Current;
				}

				Target++;
				Count++;

				if (Count == Size)
				{
					break;
				}
			}
			else
			{
				Target = Substring;
				Result = nullptr;
				Finding = false;
			}

			Current++;
		}

		if (Count != Size)
		{
			Result = nullptr;
		}

		return Result;
	}

	uint64 StringCApi::Length(const char* Text)
	{
		if (!Text)
		{
			return 0;
		}

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

	bool StringUtility::Start(const StringView& Text, const StringView& Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring.C(), Substring.GetCount());
		return Result && Text.C() == Result;
	}

	bool StringUtility::End(const StringView& Text, const StringView& Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring.C(), Substring.GetCount());
		return Result && Substring.GetCount() == StringCApi::Length(Result);
	}

	bool StringUtility::Contains(const StringView& Text, const StringView& Substring, SearchMode Mode)
	{
		StringView Result = Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Contains, Mode, 0, nullptr);
		return !Result.IsEmpty();
	}

	StringView StringUtility::Find(const StringView& Text, const StringView& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::FindAll(const StringView& Text, const StringView& Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	StringView StringUtility::Split(const StringView& Text, const StringView& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::SplitAll(const StringView& Text, const StringView& Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	int64 StringUtility::ToInteger(const StringView& Text, int32 Radix)
	{
		return StringCApi::ToInteger(Text.C());
	}

	uint64 StringUtility::ToUnsignedInteger(const StringView& Text, int32 Radix)
	{
		return StringCApi::ToUnsignedInteger(Text.C());
	}

	double StringUtility::ToDouble(const StringView& Text)
	{
		return StringCApi::ToDouble(Text.C());
	}

	String StringUtility::ToStringI(int64 Number, StringView Format)
	{
		String Result = String();
		StringCApi::ToStringI(Number, Result.GetCapacity(), Result.GetData(), Format.C());
		Result.Validate();
		return Move(Result);
	}

	String StringUtility::ToStringU(uint64 Number, StringView Format)
	{
		String Result = String();
		StringCApi::ToStringU(Number, Result.GetCapacity(), Result.GetData(), Format.C());
		Result.Validate();
		return Move(Result);
	}

	String StringUtility::ToStringF(float Number, StringView Format)
	{
		String Result = String();
		StringCApi::ToStringF(Number, Result.GetCapacity(), Result.GetData(), Format.C());
		Result.Validate();
		return Move(Result);
	}

	String StringUtility::ToStringD(double Number, StringView Format)
	{
		String Result = String();
		StringCApi::ToStringD(Number, Result.GetCapacity(), Result.GetData(), Format.C());
		Result.Validate();
		return Move(Result);
	}

	String StringUtility::ToStringB(bool State, StringView Format)
	{
		String Result = String();
		StringCApi::ToStringB(State, Result.GetCapacity(), Result.GetData(), Format.C());
		Result.Validate();
		return Move(Result);
	}

	StringView StringUtility::Search(const char* Text, const char* Substring, uint64 Size, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<StringView>* Results)
	{
		uint64 Index = 0;
		const char* Previous = Text;
		const char* Pointer = Text;
		StringView Result = nullptr;

		do
		{
			switch (Mode)
			{
			case SearchMode::Substring: Pointer = StringCApi::SearchStr(Pointer, Substring, Size); break;
			case SearchMode::Characters: Pointer = StringCApi::SearchChr(Pointer, Substring); break;
			}

			switch (Behaviour)
			{
			case SearchBehaviour::Contains: Result = StringView(Pointer); break;
			case SearchBehaviour::Find: Result = StringView(Pointer); break;
			case SearchBehaviour::Split: Result = StringView(Previous, StringCApi::Length(Previous) - StringCApi::Length(Pointer)); break;
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

	String operator+(const StringView& TextA, const StringView& TextB)
	{
		String Return = String(TextA.GetCount() + TextB.GetCount());
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator-(const StringView& TextA, const StringView& TextB)
	{
		String Return = String(TextA.GetCount());
		Return.Append(TextA);
		Return.Remove(TextB, 0, 0, true);
		return Move(Return);
	}

	bool operator==(const StringView& TextA, const StringView& TextB)
	{
		return TextA.GetCount() == TextB.GetCount() && StringCApi::Compare(TextA.C(), TextB.C()) == 0;
	}

	bool operator!=(const StringView& TextA, const StringView& TextB)
	{
		return !(TextA == TextB);
	}

	bool operator>(const StringView& TextA, const StringView& TextB)
	{
		int8 Comparaison =  StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() > TextB.GetCount();
		}
		return Comparaison > 0;
	}

	bool operator>=(const StringView& TextA, const StringView& TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() >= TextB.GetCount();
		}
		return Comparaison >= 0;
	}

	bool operator<(const StringView& TextA, const StringView& TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() < TextB.GetCount();
		}
		return Comparaison < 0;
	}

	bool operator<=(const StringView& TextA, const StringView& TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() <= TextB.GetCount();
		}
		return Comparaison <= 0;
	}

	StringId operator""_Sid(const char* Text, uint64 Size)
	{
		return StringId(StringView(Text, Size));
	}
}
