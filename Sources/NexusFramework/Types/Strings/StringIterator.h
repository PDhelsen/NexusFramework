#pragma once

#include "NexusFramework/Misc/Iterator.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	namespace Iterator
	{
		struct StringCharacter : public Iterator<StringView, StringCharacter>
		{
		public:
			StringCharacter(StringView Text, uint64 Offset = 0)
				: Data(Text), Cursor(Data.C(), Offset, Data.GetCount() - Offset)
			{
			}

			void Increment()
			{
				if (Cursor.C() >= Data.C() + Data.GetCount())
				{
					return;
				}

				uint64 Position = Cursor.C() - Data.C();
				Position++;
				Cursor = Data.Substring(Position, Data.GetCount() - Position);
			}

			void Decrement()
			{
				if (Cursor.C() <= Data.C())
				{
					return;
				}

				uint64 Position = Cursor.C() - Data.C();
				Position--;
				Cursor = Data.Substring(Position, Data.GetCount() - Position);
			}

			StringView& Get()
			{
				return Cursor;
			}

			const StringView& Get() const
			{
				return Cursor;
			}

			uint64 Id() const
			{
				return Cursor.C() - Data.C();
			}

			bool Equals(const StringCharacter& Other) const
			{
				return Data == Other.Data && Cursor == Other.Cursor;
			}

		private:
			StringView Data;
			StringView Cursor;
		};

		struct StringToken : public Iterator<StringView, StringToken>
		{
		public:
			StringToken(StringView Token, StringView Text, uint64 Offset = 0)
				: Token(Token), Data(Text), Cursor(Data.C(), Offset, 0)
			{
				Increment();
			}

			void Increment()
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

			void Decrement()
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

			StringView& Get()
			{
				return Cursor;
			}

			const StringView& Get() const
			{
				return Cursor;
			}

			uint64 Id() const
			{
				return Cursor.C() - Data.C();
			}

			bool Equals(const StringToken& Other) const
			{
				return Token == Other.Token && Data == Other.Data && Cursor == Other.Cursor;
			}

		private:
			StringView Token;
			StringView Data;
			StringView Cursor;
		};
	}
}
