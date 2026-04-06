#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringIterator.h"

namespace NxFr
{
	namespace Iterator
	{
		StringCharacter::StringCharacter(StringView Text, uint64 Offset)
			: Data(Text), Cursor(Offset)
		{
		}

		StringCharacter::~StringCharacter()
		{
		}

		StringCharacter& StringCharacter::operator++()
		{
			Iterate();
			return *this;
		}

		StringCharacter StringCharacter::operator++(int32)
		{
			StringCharacter Temp = *this;
			++(*this);
			return Temp;
		}

		StringCharacter& StringCharacter::operator--()
		{
			Reverse();
			return *this;
		}

		StringCharacter StringCharacter::operator--(int32)
		{
			StringCharacter Temp = *this;
			--(*this);
			return Temp;
		}

		StringView StringCharacter::operator->() const
		{
			return Get();
		}

		StringView StringCharacter::operator*() const
		{
			return Get();
		}

		bool StringCharacter::operator==(const StringCharacter& Other)
		{
			return Equals(Other);
		}

		bool StringCharacter::operator!=(const StringCharacter& Other)
		{
			return !Equals(Other);
		}

		bool StringCharacter::Equals(const StringCharacter& Other) const
		{
			return Data == Other.Data && Cursor == Other.Cursor;
		}

		StringView StringCharacter::GetRemainder() const
		{
			return Cursor >= 0 && Cursor < Data.GetCount() ? Data.Substring(Cursor, Data.GetCount() - Cursor) : Data.Substring(0, 0);
		}

		StringView StringCharacter::Get() const
		{
			return Cursor >= 0 && Cursor < Data.GetCount() ? Data.Substring(Cursor, 1) : Data.Substring(0, 0);
		}

		uint64 StringCharacter::Id() const
		{
			return Cursor;
		}

		void StringCharacter::Iterate()
		{
			if (Cursor >= Data.GetCount())
			{
				return;
			}

			++Cursor;
		}

		void StringCharacter::Reverse()
		{
			if (Cursor <= 0)
			{
				return;
			}

			--Cursor;
		}

		StringCharacter& StringCharacter::Next()
		{
			Iterate();
			return *this;
		}

		StringCharacter& StringCharacter::Previous()
		{
			Reverse();
			return *this;
		}

		StringToken::StringToken(StringView Token, StringView Text, uint64 Offset)
			: Token(Token), Data(Text), Cursor(Data.C(), Offset, 0)
		{
			Iterate();
		}

		StringToken::~StringToken()
		{
		}

		StringToken& StringToken::operator++()
		{
			Iterate();
			return *this;
		}

		StringToken StringToken::operator++(int32)
		{
			StringToken Temp = *this;
			++(*this);
			return Temp;
		}

		StringToken& StringToken::operator--()
		{
			Reverse();
			return *this;
		}

		StringToken StringToken::operator--(int32)
		{
			StringToken Temp = *this;
			--(*this);
			return Temp;
		}

		StringView StringToken::operator->() const
		{
			return Get();
		}

		StringView StringToken::operator*() const
		{
			return Get();
		}

		bool StringToken::operator==(const StringToken& Other)
		{
			return Equals(Other);
		}

		bool StringToken::operator!=(const StringToken& Other)
		{
			return !Equals(Other);
		}

		bool StringToken::Equals(const StringToken& Other) const
		{
			return Token == Other.Token && Data == Other.Data && Cursor == Other.Cursor;
		}

		StringView StringToken::GetToken() const
		{
			return Token;
		}

		StringView StringToken::GetRemainder() const
		{
			return !Cursor.IsEmpty() ? Data.Substring(Cursor.C() - Data.C(), Data.GetCount() - (Cursor.C() - Data.C())) : Data.Substring(0, 0);
		}

		StringView StringToken::Get() const
		{
			return Cursor;
		}

		uint64 StringToken::Id() const
		{
			return Cursor.C() - Data.C();
		}

		void StringToken::Iterate()
		{
			const char* Start = Cursor.C() + (Cursor.GetCount() != 0 ? Cursor.GetCount() + Token.GetCount() : 0);
			const char* End = Data.C() + Data.GetCount();

			const char* Pointer = Start;
			while (StringCApi::Compare(Pointer, Token.C(), Token.GetCount()) != 0 && Pointer < End)
			{
				++Pointer;
			}

			Cursor = StringView(Start, Pointer - Start);
		}

		void StringToken::Reverse()
		{
			const char* Start = Cursor.C() - (Token.GetCount() + 1);
			const char* End = Data.C();

			const char* Pointer = Start;
			while (StringCApi::Compare(Pointer, Token.C(), Token.GetCount()) != 0 && Pointer > End)
			{
				--Pointer;
			}

			if (Pointer > End)
			{
				Pointer += Token.GetCount();
			}
			if (Start > End)
			{
				Start++;
			}

			Cursor = StringView(Pointer, Start - Pointer);
		}

		StringToken& StringToken::Next()
		{
			Iterate();
			return *this;
		}

		StringToken& StringToken::Previous()
		{
			Reverse();
			return *this;
		}	
	}
}
