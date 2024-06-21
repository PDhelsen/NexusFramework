#include "Core/NexusEnginePch.h"
#include "String.h"

namespace NxEn
{
	String::String()
		: Allocator(nullptr), Capacity(0), Count(0), Data(nullptr)
	{
	}

	String::String(uint64 Bytes)
	{
		Allocate(Bytes, 0, nullptr);
	}

	String::String(const char* Text)
	{
		uint64 Size = Length(Text);
		Allocate(Size, Size, Text);
	}

	String::String(const String& Other)
	{
		Allocate(Other.Capacity, Other.Count, Other.Data);
	}

	String::String(String&& Other) noexcept
		: Allocator(Other.Allocator), Capacity(Other.Capacity), Count(Other.Count), Data(Other.Data)
	{
		Other.Allocator = nullptr;
		Other.Capacity = 0;
		Other.Count = 0;
		Other.Data = nullptr;
	}

	String::~String()
	{
		Free();
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
		Append(Text.Data, Text.Count);
		return *this;
	}

	String& String::Append(const char* Text)
	{
		Append(Text, Length(Text));
		return *this;
	}

	String& String::Assign(const String& OldText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText.Data, OldText.Count, NewText.Data, NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const String& OldText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText.Data, OldText.Count, NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const char* OldText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText, Length(OldText), NewText.Data, NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Assign(const char* OldText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Assign(OldText, Length(OldText), NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const String& ReferenceText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.Data, ReferenceText.Count, NewText.Data, NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const String& ReferenceText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText.Data, ReferenceText.Count, NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const char* ReferenceText, const String& NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText, Length(ReferenceText), NewText.Data, NewText.Count, Offset, Occurrence, All);
		return *this;
	}

	String& String::Insert(const char* ReferenceText, const char* NewText, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Insert(ReferenceText, Length(ReferenceText), NewText, Length(NewText), Offset, Occurrence, All);
		return *this;
	}

	String& String::Remove(const String& Text, uint64 Offset /*0*/, uint64 Occurrence /*1*/, bool All /*false*/)
	{
		Remove(Text.Data, Text.Count, Offset, Occurrence, All);
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

	int8 String::Compare(const String& Substring) const
	{
		return Compare(Data, Substring.Data);
	}

	int8 String::Compare(const char* Substring) const
	{
		return Compare(Data, Substring);
	}

	bool String::Start(const String& Substring) const
	{
		const char* Result = SearchStr(Data, Substring.Data);
		return Result && Data == Result;
	}

	bool String::Start(const char* Substring) const
	{
		const char* Result = SearchStr(Data, Substring);
		return Result && Data == Result;
	}

	bool String::End(const String& Substring) const
	{
		const char* Result = SearchStr(Data, Substring.Data);
		return Result && Substring.Count == Length(Result);
	}

	bool String::End(const char* Substring) const
	{
		const char* Result = SearchStr(Data, Substring);
		return Result && Length(Substring) == Length(Result);
	}

	bool String::Contains(const String& Substring, SearchMode Mode /*SearchMode::Substring*/) const
	{
		return Search(Substring.Data, SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	bool String::Contains(const char* Substring, SearchMode Mode /*SearchMode::Substring*/) const
	{
		return Search(Substring, SearchBehaviour::Contains, Mode, 0, nullptr) != nullptr;
	}

	const char* String::Find(const String& Substring, uint64 Offset /*0*/, SearchMode Mode /*SearchMode::Substring*/) const
	{
		return Search(Substring.Data, SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	const char* String::Find(const char* Substring, uint64 Offset /*0*/, SearchMode Mode /*SearchMode::Substring*/) const
	{
		return Search(Substring, SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<const char*> String::FindAll(const String& Substring, SearchMode Mode /*SearchMode::Substring*/) const
	{
		List<const char*> Results;
		Search(Substring.Data, SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::FindAll(const char* Substring, SearchMode Mode /*SearchMode::Substring*/) const
	{
		List<const char*> Results;
		Search(Substring, SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	const char* String::Split(const String& Substring, uint64 Offset /*0*/, SearchMode Mode /*SearchMode::Substring*/) const
	{
		return Search(Substring.Data, SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	const char* String::Split(const char* Substring, uint64 Offset /*0*/, SearchMode Mode /*SearchMode::Substring*/) const
	{
		return Search(Substring, SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<const char*> String::SplitAll(const String& Substring, SearchMode Mode /*SearchMode::Substring*/) const
	{
		List<const char*> Results;
		Search(Substring.Data, SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	List<const char*> String::SplitAll(const char* Substring, SearchMode Mode /*SearchMode::Substring*/) const
	{
		List<const char*> Results;
		Search(Substring, SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
	}

	void String::Allocate(uint64 Bytes, uint64 Size, const char* Text)
	{
		ValidateCapacityCount(Bytes, Size);

		Allocator = Memory::GetActiveAllocator();
		Data = (char*)Memory::Allocate(Capacity, NEXUS_MEMORY_ALIGN, Allocator);

		if (Text)
		{
			Memory::MemCopy(Text, Data, Capacity);
		}

		ValidateNullTermination();
	}

	void String::Reallocate(uint64 Bytes)
	{
		ValidateCapacityCount(Bytes, Count);
		Data = (char*)Memory::Realloc(Data, Capacity, NEXUS_MEMORY_ALIGN, Allocator);
		ValidateNullTermination();
	}

	void String::Free()
	{
		Memory::Free(Data, Allocator);
	}

	void String::Append(const char* Text, uint64 Size)
	{
		Resize(Count + Size);
		Concat(Text, Data, Capacity);
	}

	void String::Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset, uint64 Occurrence, bool All)
	{
		if (OldSize <= 0 || NewSize <= 0 || Compare(OldText, NewText) == 0)
		{
			return;
		}

		uint64 Index = 0;
		uint64 Modified = 0;
		char* Substring = Data;
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
		char* Substring = Data;

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
		char* Substring = Data;

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

	void String::ValidateCapacityCount(uint64 Bytes, uint64 Size)
	{
		Count = Size;
		Capacity = Bytes >= Count + 1 ? Bytes : Count + 1;
	}

	void String::ValidateNullTermination()
	{
		Data[Count] = NullChar;
		Data[Capacity - 1] = NullChar;
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

	const char* String::Search(const char* Substring, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<const char*>* Results) const
	{
		uint64 Index = 0;
		const char* Previous = Data;
		const char* Pointer = Data;
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
}
