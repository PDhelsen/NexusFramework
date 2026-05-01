#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"

namespace NxFr
{
#pragma region Functionality

	bool StringUtility::Start(StringView Text, StringView Substring)
	{
		if (Text.GetCount() < Substring.GetCount())
		{
			return false;
		}

		return StringCApi::Compare(Text.C(), Substring.C(), Substring.GetCount()) == 0;
	}

	bool StringUtility::End(StringView Text, StringView Substring)
	{
		if (Text.GetCount() < Substring.GetCount())
		{
			return false;
		}

		StringView View = Text.Substring(Text.GetCount() - Substring.GetCount(), Substring.GetCount());
		return StringCApi::Compare(View.C(), Substring.C(), Substring.GetCount()) == 0;
	}

	bool StringUtility::Contains(StringView Text, StringView Substring, SearchMode Mode)
	{
		StringView Result = Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Contains, Mode, 0, nullptr);
		return !Result.IsEmpty();
	}

	StringView StringUtility::Common(StringView Text1, StringView Text2)
	{
		uint64 Count = StringCApi::Common(Text1.C(), Text2.C());
		return StringView(Text1.C(), Count);
	}

	StringView StringUtility::Find(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::FindAll(StringView Text, StringView Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Find, Mode, 0, &Results);
		return Results;
	}

	StringView StringUtility::Split(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::SplitAll(StringView Text, StringView Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Text.GetCount(), Substring.GetCount(), SearchBehaviour::Split, Mode, 0, &Results);
		return Results;
	}

	StringView StringUtility::TrimLeading(StringView Text, char Character)
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

	StringView StringUtility::TrimTrailing(StringView Text, char Character)
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

	String StringUtility::Lower(StringView Text)
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

	String StringUtility::Upper(StringView Text)
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

	String StringUtility::Replace(StringView Text, StringView Old, StringView New)
	{
		return String(Text).Assign(Old, New);
	}

	String StringUtility::Join(const Collection<StringView>& Text, StringView Separator)
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

	StringView StringUtility::Search(const char* Text, const char* Substring, uint64 Capacity, uint64 Size, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<StringView>* Results)
	{
		uint64 Index = 0;
		const char* Previous = Text;
		const char* Pointer = Text;
		StringView Result;

		do
		{
			switch (Mode)
			{
			case SearchMode::Substring: Pointer = StringCApi::SearchStr(Pointer, Substring, Capacity - (Pointer - Text), Size); break;
			case SearchMode::Characters: Pointer = StringCApi::SearchChr(Pointer, Substring); break;
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

#pragma endregion

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
