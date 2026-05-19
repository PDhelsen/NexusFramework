#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/String.h"

#include "NexusFramework/Memory/Memory.h"

namespace NxFr
{
	String::String(Allocator* Allctr)
		: Alloc(Allctr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(SmallStringCapacity, 0, nullptr);
	}

	String::String(uint64 Bytes, Allocator* Allctr)
		: Alloc(Allctr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Bytes, 0, nullptr);
	}

	String::String(const char* Text, Allocator* Allctr)
		: Alloc(Allctr), Capacity(SmallStringCapacity), Count(0)
	{
		uint64 Size = StringCApi::Length(Text);
		Allocate(Size, Size, Text);
	}

	String::String(const char* Text, uint64 Size, Allocator* Allctr)
		: Alloc(Allctr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Size, Size, Text);
	}

	String::String(StringView Text, Allocator* Allctr)
		: Alloc(Allctr), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Text.GetCount(), Text.GetCount(), Text.C());
	}

	String::String(const String& Other)
		: Alloc(Other.Alloc), Capacity(SmallStringCapacity), Count(0)
	{
		Allocate(Other.Capacity, Other.Count, Other.GetBuffer());
	}

	String::String(String&& Other) noexcept
		: Alloc(Other.Alloc), Capacity(Other.Capacity), Count(Other.Count)
	{
		if (Other.Sso())
		{
			StringCApi::Copy(Other.Data.Small, Data.Small, SmallStringCapacity);
		}
		else
		{
			Data.Large = Other.Data.Large;
		}

		Other.Count = 0;
		Other.Capacity = SmallStringCapacity;
		Other.Data.Large = nullptr;

		ValidateNullTermination();
	}

	String::~String()
	{
		Free();
	}

	String& String::operator=(const String& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Resize(Other.Count);
		StringCApi::Copy(Other.GetBuffer(), GetData(), Capacity);
		return *this;
	}

	String& String::operator=(String&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Free();

		Alloc = Other.Alloc;
		Capacity = Other.Capacity;
		Count = Other.Count;

		if (Other.Sso())
		{
			StringCApi::Copy(Other.Data.Small, Data.Small, SmallStringCapacity);
		}
		else
		{
			Data.Large = Other.Data.Large;
		}

		Other.Count = 0;
		Other.Capacity = SmallStringCapacity;
		Other.Data.Large = nullptr;

		ValidateNullTermination();

		return *this;
	}

	String& String::operator+=(StringView Other)
	{
		Append(Other);
		return *this;
	}

	String& String::operator-=(StringView Other)
	{
		Remove(Other);
		return *this;
	}

	String& String::Assign(StringView OldText, StringView NewText)
	{
		Assign(OldText.C(), OldText.GetCount(), NewText.C(), NewText.GetCount(), 0, 0, true);
		return *this;
	}

	String& String::Assign(StringView OldText, StringView NewText, uint64 Offset, uint64 Occurrence)
	{
		Assign(OldText.C(), OldText.GetCount(), NewText.C(), NewText.GetCount(), Offset, Occurrence, false);
		return *this;
	}

	String& String::Append(StringView Text)
	{
		Append(Text.C(), Text.GetCount());
		return *this;
	}

	String& String::Insert(StringView ReferenceText, StringView NewText)
	{
		Insert(ReferenceText.C(), ReferenceText.GetCount(), NewText.C(), NewText.GetCount(), 0, 0, true);
		return *this;
	}

	String& String::Insert(StringView ReferenceText, StringView NewText, uint64 Offset, uint64 Occurrence)
	{
		Insert(ReferenceText.C(), ReferenceText.GetCount(), NewText.C(), NewText.GetCount(), Offset, Occurrence, false);
		return *this;
	}

	String& String::Remove(StringView Text)
	{
		Remove(Text.C(), Text.GetCount(), 0, 0, true);
		return *this;
	}

	String& String::Remove(StringView Text, uint64 Offset, uint64 Occurrence)
	{
		Remove(Text.C(), Text.GetCount(), Offset, Occurrence, false);
		return *this;
	}

	String& String::Terminate(uint64 Size)
	{
		Resize(Size);
		return *this;
	}

	String& String::Clear()
	{
		Resize(0);
		return *this;
	}

	void String::Reserve(uint64 Size)
	{
		Size = Math::Max(Size, Count + 1ull);
		Size = Math::Max(Size, SmallStringCapacity + 1ull);
		Reallocate(Size);
	}

	void String::Validate()
	{
		Count = StringCApi::Length(C());
		ValidateNullTermination();
	}

	const Iterator::StringCharacter String::Begin() const
	{
		return Iterator::StringCharacter(*this, 0);
	}

	const Iterator::StringToken String::Begin(StringView Token) const
	{
		return Iterator::StringToken(Token, *this, 0);
	}

	const Iterator::StringCharacter String::End() const
	{
		return Iterator::StringCharacter(*this, GetCount());
	}

	const Iterator::StringToken String::End(StringView Token) const
	{
		return Iterator::StringToken(Token, *this, GetCount());
	}

	StringView String::Substring(uint64 Offset, uint64 Size) const
	{
		return StringView(C(), Offset, Size);
	}

	char* String::Characters()
	{
		return GetData();
	}

	void String::Allocate(uint64 Bytes, uint64 Size, const char* Text)
	{
		ValidateCapacityCount(Bytes, Size);

		if (!Sso())
		{
			Data.Large = (char*)Memory::Allocate(Capacity, Alloc);
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
			Data.Large = (char*)Memory::Reallocate(Data.Large, Capacity, Alloc);
		}
		else if (!Sso() && WasSso)
		{
			char Temp[SmallStringCapacity];
			StringCApi::Copy(Data.Small, Temp, SmallStringCapacity);

			Data.Large = (char*)Memory::Allocate(Capacity, Alloc);
			StringCApi::Copy(Temp, Data.Large, Capacity);
		}
		else
		{
			// Once grow over the Sso limit, it cannot go back to Sso.
			// The cost of allocating has been paid, so there is no point to freeing the memory until the string is destroyed
			// The only exception is when using the move constructor / operator
			NX_ASSERT(false, Default, "Not supposed to reallocate from sso to sso");
		}

		ValidateNullTermination();
	}

	void String::Free()
	{
		if (!Sso())
		{
			Memory::Free(Data.Large, Alloc);
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
		if (OldSize <= 0 || (OldSize == NewSize && StringCApi::Compare(OldText, NewText, OldSize) == 0))
		{
			return;
		}

		uint64 Index = 0;
		uint64 Modified = 0;
		char* Substring = GetData();
		int64 SizeDiff = NewSize - OldSize;

		do
		{
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, OldText, Count, OldSize));

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
							Memory::MemMove(Substring + OldSize, Substring + NewSize, Remaining);
						}
					}
					else if (SizeDiff < 0)
					{
						uint64 Remaining = StringCApi::Length(Substring + OldSize);
						if (Remaining > 0)
						{
							Memory::MemMove(Substring + OldSize, Substring + NewSize, Remaining);
						}

						Resize(Count + SizeDiff);
					}

					StringCApi::Copy(NewText, Substring, Capacity, NewSize, true);

					++Modified;
				}

				Substring += NewSize;
			}

			++Index;
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
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, ReferenceText, Count, ReferenceSize));

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
						Memory::MemMove(Substring, Substring + NewSize, StringCApi::Length(Substring));
					}

					StringCApi::Copy(NewText, Substring, Capacity, NewSize, true);

					++Modified;
				}
			}

			++Index;
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
			Substring = const_cast<char*>(StringCApi::SearchStr(Substring, Text, Count, Size));

			if (Substring)
			{
				if (Index >= Offset)
				{
					uint64 Remaining = StringCApi::Length(Substring) - Size;
					if (Remaining > 0)
					{
						Memory::MemMove(Substring + Size, Substring, Remaining);
					}

					Resize(Count - Size);
					++Removed;
				}
				else
				{
					++Substring;
				}
			}

			++Index;
		} while (Substring && (All || (!All && Removed < Occurrence)));
	}
}
