#include "Core/NexusEnginePch.h"
#include "String.h"

namespace NxEn
{
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
					uint64 Position = Substring - GetData();

					if (SizeDiff > 0)
					{
						Resize(Count + SizeDiff);
						Substring = GetData() + Position;

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
				uint64 Position = Substring - GetData();

				if (Index >= Offset)
				{
					Resize(Count + NewSize);
					Substring = GetData() + Position;

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
}
