#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/Utility.h"

namespace NxFr
{
	enum class SearchBehaviour
	{
		Contains, Find, Split
	};

	static StringView Search(const char* Text, const char* Substring, uint64 Capacity, uint64 Size, SearchBehaviour Behaviour, StringUtility::SearchMode Mode, uint64 Offset, List<StringView>* Results)
	{
		uint64 Index = 0;
		const char* Previous = Text;
		const char* Pointer = Text;
		StringView Result;

		do
		{
			switch (Mode)
			{
			case StringUtility::SearchMode::Substring: Pointer = StringCApi::SearchStr(Pointer, Substring, Capacity - (Pointer - Text), Size); break;
			case StringUtility::SearchMode::Characters: Pointer = StringCApi::SearchChr(Pointer, Substring); break;
			}

			switch (Behaviour)
			{
			case SearchBehaviour::Contains:
			case SearchBehaviour::Find: Result = Pointer != nullptr ? StringView(Pointer, Capacity - (StringCApi::Length(Text) - StringCApi::Length(Pointer))) : StringView(); break;
			case SearchBehaviour::Split: Result = Pointer != nullptr ? StringView(Previous, StringCApi::Length(Previous) - StringCApi::Length(Pointer)) : StringView(Previous, Capacity - (StringCApi::Length(Text) - StringCApi::Length(Previous))); break;
			}

			if (Results && Result != StringUtility::Empty)
			{
				Results->Append(Result);
			}
			else if (Index == Offset)
			{
				break;
			}

			if (!Pointer)
			{
				break;
			}

			++Index;
			Pointer += Size;
			Previous = Pointer;
		} while (true);

		return Result;
	}

	namespace StringUtility
	{
		bool Start(StringView Text, StringView Substring)
		{
			if (Text.GetCount() < Substring.GetCount())
			{
				return false;
			}

			return StringCApi::Compare(Text.C(), Substring.C(), Substring.GetCount()) == 0;
		}

		bool End(StringView Text, StringView Substring)
		{
			if (Text.GetCount() < Substring.GetCount())
			{
				return false;
			}

			StringView View = Text.Substring(Text.GetCount() - Substring.GetCount(), Substring.GetCount());
			return StringCApi::Compare(View.C(), Substring.C(), Substring.GetCount()) == 0;
		}

		bool Contains(StringView Text, StringView Substring, SearchMode Mode)
		{
			StringView Result = Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Contains, Mode, 0, nullptr);
			return !Result.IsEmpty();
		}

		StringView Common(StringView Text1, StringView Text2)
		{
			uint64 Count = StringCApi::Common(Text1.C(), Text2.C());
			return StringView(Text1.C(), Count);
		}

		StringView Find(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
		{
			return Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Find, Mode, Offset, nullptr);
		}

		List<StringView> FindAll(StringView Text, StringView Substring, SearchMode Mode)
		{
			List<StringView> Results;
			Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Find, Mode, 0, &Results);
			return Results;
		}

		StringView Split(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
		{
			return Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Split, Mode, Offset, nullptr);
		}

		List<StringView> SplitAll(StringView Text, StringView Substring, SearchMode Mode)
		{
			List<StringView> Results;
			Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Split, Mode, 0, &Results);
			return Results;
		}

		List<StringView> Tokenize(StringView Text, StringView Separator)
		{
			List<StringView> Result;

			if (Text.IsEmpty() || Separator.IsEmpty())
			{
				return Result;
			}

			static const char* QuoteChar = "\"";
			static uint64 QuoteSize = 1;
			bool Quotes = false;

			const char* Pointer = Text.C();
			const char* Start = Text.C();
			const char* End = Text.C() + Text.GetCount();

			while (Pointer != End)
			{
				if (StringCApi::Compare(Pointer, QuoteChar, QuoteSize) == 0)
				{
					if (!Quotes)
					{
						StringView Token = StringView(Start, Pointer - Start);
						if (!Token.IsEmpty())
						{
							Result.Append(Token);
						}

						Quotes = true;
					}
					else
					{
						StringView Token = StringView(Start, Pointer - Start);
						Result.Append(Token);

						Quotes = false;
					}

					Pointer += QuoteSize;
					Start = Pointer;
					continue;
				}
				else if (!Quotes && uint64(End - Pointer) >= Separator.GetCount() && StringCApi::Compare(Pointer, Separator.C(), Separator.GetCount()) == 0)
				{
					StringView Token = StringView(Start, Pointer - Start);
					if (!Token.IsEmpty())
					{
						Result.Append(Token);
					}

					Pointer += Separator.GetCount();
					Start = Pointer;
					continue;
				}
				else
				{
					Pointer++;
				}
			}

			StringView Token = StringView(Start, Pointer - Start);
			if (!Token.IsEmpty())
			{
				Result.Append(Token);
			}

			NX_ASSERT(!Quotes, Default, "Malformed string with unmatched quotes");

			return Result;
		}

		StringView TrimLeading(StringView Text, char Character)
		{
			uint64 Index = 0;
			uint64 Count = Text.GetCount();
			const char* Position = Text.C();

			while (Index < Count && *Position == Character)
			{
				Position++;
				Index++;
			}

			return StringView(Position, Count - Index);
		}

		StringView TrimTrailing(StringView Text, char Character)
		{
			uint64 Count = Text.GetCount();
			const char* Position = Text.C() + Count - 1;

			while (Count >= 0 && *Position == Character)
			{
				Position--;
				Count--;
			}

			return StringView(Text.C(), Count);
		}

		String Lower(StringView Text)
		{
			String Result = Text.GetCount() + 1;

			const char* Position = Text.C();
			while (*Position != NullChar)
			{
				char Character = (*Position >= 'A' && *Position <= 'Z') ? (*Position + 32) : *Position;
				Result += StringView(&Character, 1);

				Position++;
			}

			Result.Terminate(Text.GetCount());
			return Result;
		}

		String Upper(StringView Text)
		{
			String Result = Text.GetCount() + 1;

			const char* Position = Text.C();
			while (*Position != NullChar)
			{
				char Character = (*Position >= 'a' && *Position <= 'z') ? (*Position - 32) : *Position;
				Result += StringView(&Character, 1);

				Position++;
			}

			Result.Terminate(Text.GetCount());
			return Result;
		}

		String Replace(StringView Text, StringView Old, StringView New)
		{
			return String(Text).Assign(Old, New);
		}

		String Join(const Collection<StringView>& Text, StringView Separator)
		{
			String Result;
			for (auto It = Text.Begin(); It != Text.End(); ++It)
			{
				Result += *It;
				if (!Separator.IsEmpty())
				{
					Result += Separator;
				}
			}
			return Result;
		}
	}

#pragma region Operator

	String operator+(StringView TextA, StringView TextB)
	{
		String Return = String(TextA.GetCount() + TextB.GetCount());
		Return.Append(TextA);
		Return.Append(TextB);
		return Return;
	}

	String operator-(StringView TextA, StringView TextB)
	{
		String Return = String(TextA.GetCount());
		Return.Append(TextA);
		Return.Remove(TextB);
		return Return;
	}

	bool operator==(StringView TextA, StringView TextB)
	{
		return TextA.GetCount() == TextB.GetCount() && StringCApi::Compare(TextA.C(), TextB.C(), TextA.GetCount()) == 0;
	}

	bool operator!=(StringView TextA, StringView TextB)
	{
		return !(TextA == TextB);
	}

	bool operator>(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C(), Math::Min(TextA.GetCount(), TextB.GetCount()));
		return Comparaison > 0 || (Comparaison == 0 && TextA.GetCount() > TextB.GetCount());
	}

	bool operator>=(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C(), Math::Min(TextA.GetCount(), TextB.GetCount()));
		return Comparaison >= 0 || (Comparaison == 0 && TextA.GetCount() >= TextB.GetCount());
	}

	bool operator<(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C(), Math::Min(TextA.GetCount(), TextB.GetCount()));
		return Comparaison < 0 || (Comparaison == 0 && TextA.GetCount() < TextB.GetCount());
	}

	bool operator<=(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C(), Math::Min(TextA.GetCount(), TextB.GetCount()));
		return Comparaison <= 0 || (Comparaison == 0 && TextA.GetCount() <= TextB.GetCount());
	}

#pragma endregion
}
