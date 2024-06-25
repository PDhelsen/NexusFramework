#include "Core/NexusEnginePch.h"
#include "String.h"

namespace NxEn
{
	String String::Empty = String();

	String::String()
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(SmallStringCapacity, 0, nullptr, Memory::GetActiveAllocator());
	}

	String::String(uint64 Bytes)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Bytes, 0, nullptr, Memory::GetActiveAllocator());
	}

	String::String(const char* Text)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		uint64 Size = Length(Text);
		Allocate(Size, Size, Text, Memory::GetActiveAllocator());
	}

	String::String(const String& Other)
		: Allctr(nullptr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Other.Capacity, Other.Count, Other.GetBuffer(), Other.Allctr);
	}

	String::String(String&& Other) noexcept
		: Allctr(Other.Allctr), Capacity(Other.Capacity), Count(Other.Count)
	{
		if (Other.Sso())
		{
			Copy(Other.Data.Small, Data.Small, SmallStringCapacity);
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
		Copy(Other.GetBuffer(), GetData(), Other.Capacity);
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

	String operator+(const String& TextA, const String& TextB)
	{
		String Return = String(TextA.GetCount() + TextB.GetCount());
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator+(const String& TextA, const char* TextB)
	{
		String Return = String(TextA.GetCount() + String::Length(TextB));
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator+(const char* TextA, const String& TextB)
	{
		String Return = String(String::Length(TextA) + TextB.GetCount());
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
		String Return = String(String::Length(TextA));
		Return.Append(TextA);
		Return.Remove(TextB, 0, 0, true);
		return Move(Return);
	}

	bool operator==(const String& TextA, const String& TextB)
	{
		return TextA.GetCount() == TextB.GetCount() && String::Compare(TextA.C(), TextB.C()) == 0;
	}

	bool operator==(const String& TextA, const char* TextB)
	{
		return String::Compare(TextA.C(), TextB) == 0;
	}

	bool operator==(const char* TextA, const String& TextB)
	{
		return String::Compare(TextA, TextB.C()) == 0;
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
		return String::Compare(TextA.C(), TextB.C()) > 0;
	}

	bool operator>(const String& TextA, const char* TextB)
	{
		return String::Compare(TextA.C(), TextB) > 0;
	}

	bool operator>(const char* TextA, const String& TextB)
	{
		return String::Compare(TextA, TextB.C()) > 0;
	}

	bool operator>=(const String& TextA, const String& TextB)
	{
		return String::Compare(TextA.C(), TextB.C()) >= 0;
	}

	bool operator>=(const String& TextA, const char* TextB)
	{
		return String::Compare(TextA.C(), TextB) >= 0;
	}

	bool operator>=(const char* TextA, const String& TextB)
	{
		return String::Compare(TextA, TextB.C()) >= 0;
	}

	bool operator<(const String& TextA, const String& TextB)
	{
		return String::Compare(TextA.C(), TextB.C()) < 0;
	}

	bool operator<(const String& TextA, const char* TextB)
	{
		return String::Compare(TextA.C(), TextB) < 0;
	}

	bool operator<(const char* TextA, const String& TextB)
	{
		return String::Compare(TextA, TextB.C()) < 0;
	}

	bool operator<=(const String& TextA, const String& TextB)
	{
		return String::Compare(TextA.C(), TextB.C()) <= 0;
	}

	bool operator<=(const String& TextA, const char* TextB)
	{
		return String::Compare(TextA.C(), TextB) <= 0;
	}

	bool operator<=(const char* TextA, const String& TextB)
	{
		return String::Compare(TextA, TextB.C()) <= 0;
	}

	String& String::Append(const String& Text)
	{
		Append(Text.GetBuffer(), Text.Count);
		return *this;
	}

	String& String::Append(const char* Text)
	{
		Append(Text, Length(Text));
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
		Assign(OldText.GetBuffer(), OldText.Count, NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const char* OldText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText, Length(OldText), NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const char* OldText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText, Length(OldText), NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const String& ReferenceText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.GetBuffer(), ReferenceText.Count, NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const String& ReferenceText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.GetBuffer(), ReferenceText.Count, NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const char* ReferenceText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText, Length(ReferenceText), NewText.GetBuffer(), NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const char* ReferenceText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText, Length(ReferenceText), NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Remove(const String& Text, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Remove(Text.GetBuffer(), Text.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Remove(const char* Text, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Remove(Text, Length(Text), Offset, Occurrence, All);
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

	int8 String::Compare(const String& Text, const String& Substring)
	{
		return Compare(Text.GetBuffer(), Substring.GetBuffer());
	}

	int8 String::Compare(const String& Text, const char* Substring)
	{
		return Compare(Text.GetBuffer(), Substring);
	}

	int8 String::Compare(const char* Text, const String& Substring)
	{
		return Compare(Text, Substring.GetBuffer());
	}

	bool String::Start(const String& Text, const String& Substring)
	{
		const char* Result = SearchStr(Text.GetBuffer(), Substring.GetBuffer());
		return Result && Text.GetBuffer() == Result;
	}

	bool String::Start(const String& Text, const char* Substring)
	{
		const char* Result = SearchStr(Text.GetBuffer(), Substring);
		return Result && Text.GetBuffer() == Result;
	}

	bool String::Start(const char* Text, const String& Substring)
	{
		const char* Result = SearchStr(Text, Substring.GetBuffer());
		return Result && Text == Result;
	}

	bool String::Start(const char* Text, const char* Substring)
	{
		const char* Result = SearchStr(Text, Substring);
		return Result && Text == Result;
	}

	bool String::End(const String& Text, const String& Substring)
	{
		const char* Result = SearchStr(Text.GetBuffer(), Substring.GetBuffer());
		return Result && Substring.Count == Length(Result);
	}

	bool String::End(const String& Text, const char* Substring)
	{
		const char* Result = SearchStr(Text.GetBuffer(), Substring);
		return Result && Length(Substring) == Length(Result);
	}

	bool String::End(const char* Text, const String& Substring)
	{
		const char* Result = SearchStr(Text, Substring.GetBuffer());
		return Result && Substring.Count == Length(Result);
	}

	bool String::End(const char* Text, const char* Substring)
	{
		const char* Result = SearchStr(Text, Substring);
		return Result && Length(Substring) == Length(Result);
	}

	bool String::Contains(const String& Text, const String& Substring, SearchMode Mode)
	{
		return Search(Text.GetBuffer(), Substring.GetBuffer(), SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool String::Contains(const String& Text, const char* Substring, SearchMode Mode)
	{
		return Search(Text.GetBuffer(), Substring, SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool String::Contains(const char* Text, const String& Substring, SearchMode Mode)
	{
		return Search(Text, Substring.GetBuffer(), SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool String::Contains(const char* Text, const char* Substring, SearchMode Mode)
	{
		return Search(Text, Substring, SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	const char* String::Find(const String& Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.GetBuffer(), Substring.GetBuffer(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* String::Find(const String& Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.GetBuffer(), Substring, SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* String::Find(const char* Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring.GetBuffer(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* String::Find(const char* Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring, SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<const char*> String::FindAll(const String& Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.GetBuffer(), Substring.GetBuffer(), SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::FindAll(const String& Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.GetBuffer(), Substring, SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::FindAll(const char* Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring.GetBuffer(), SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::FindAll(const char* Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring, SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	const char* String::Split(const String& Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.GetBuffer(), Substring.GetBuffer(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* String::Split(const String& Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.GetBuffer(), Substring, SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* String::Split(const char* Text, const String& Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring.GetBuffer(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* String::Split(const char* Text, const char* Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text, Substring, SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<const char*> String::SplitAll(const String& Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.GetBuffer(), Substring.GetBuffer(), SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::SplitAll(const String& Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text.GetBuffer(), Substring, SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::SplitAll(const char* Text, const String& Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring.GetBuffer(), SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::SplitAll(const char* Text, const char* Substring, SearchMode Mode)
	{
		List<const char*> Results;
		Search(Text, Substring, SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	void String::Allocate(uint64 Bytes, uint64 Size, const char* Text, Allocator* Al)
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
			Copy(Data.Small, Temp, SmallStringCapacity);

			Data.Large = (char*)Memory::Allocate(Capacity, NEXUS_MEMORY_ALIGN, Allctr);
			Copy(Temp, Data.Large, Capacity);
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
		GetData()[Count] = NullChar;
		GetData()[Capacity - 1] = NullChar;
	}

	void String::Append(const char* Text, uint64 Size)
	{
		Resize(Count + Size);
		Concat(Text, GetData(), Capacity);
	}

	void String::Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset, uint64 Occurrence, bool All)
	{
		if (OldSize <= 0 || NewSize <= 0 || Compare(OldText, NewText) == 0)
		{
			return;
		}

		uint64 Index = 0;
		uint64 Modified = 0;
		char* Substring = GetData();
		int64 SizeDiff = NewSize - OldSize;

		do
		{
			Substring = const_cast<char*>(SearchStr(Substring, OldText));

			if (Substring)
			{
				if (Index >= Offset)
				{
					if (SizeDiff > 0)
					{
						Resize(Count + SizeDiff);
						Memory::MemCopy(Substring + OldSize, Substring + NewSize, Length(Substring));
					}
					else if (SizeDiff < 0)
					{
						Memory::MemCopy(Substring + OldSize, Substring + NewSize, Length(Substring));
						Resize(Count + SizeDiff);
					}

					Copy(NewText, Substring, Capacity, NewSize, true);

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
			Substring = const_cast<char*>(SearchStr(Substring, ReferenceText));

			if (Substring)
			{
				Substring += ReferenceSize;

				if (Index >= Offset)
				{
					Resize(Count + NewSize);
					Memory::MemCopy(Substring, Substring + NewSize, Length(Substring));
					Copy(NewText, Substring, Capacity, NewSize, true);

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
			Substring = const_cast<char*>(SearchStr(Substring, Text));

			if (Substring)
			{
				if (Index >= Offset)
				{
					uint64 Remaining = Length(Substring) - Size;
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
		}
		while (Substring && (All || (!All && Removed < Occurrence)));
	}

	const char* String::Search(const char* Text, const char* Substring, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<const char*>* Results)
	{
		uint64 Index = 0;
		const char* Previous = Text;
		const char* Pointer = Text;
		const char* Result = nullptr;

		do
		{
			switch (Mode)
			{
			case NxEn::String::SearchMode::Substring: Pointer = SearchStr(Pointer, Substring); break;
			case NxEn::String::SearchMode::Characters: Pointer = SearchChr(Pointer, Substring); break;
			}

			switch (Behaviour)
			{
			case NxEn::String::SearchBehaviour::Contains: Result = Pointer; break;
			case NxEn::String::SearchBehaviour::Find: Result = Pointer; break;
			case NxEn::String::SearchBehaviour::Split: Result = Previous; break;
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
		}
		while (true);

		if (Results && Behaviour == SearchBehaviour::Split)
		{
			Results->Append(Result);
		}

		return Result;
	}
}
