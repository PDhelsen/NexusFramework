#include "Core/NexusEnginePch.h"
#include "StringFunctions.h"

namespace NxEn
{
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

		StringView View = Text.ToView(Text.GetCount() - Substring.GetCount(), Substring.GetCount());
		return StringCApi::Compare(View.C(), Substring.C(), Substring.GetCount()) == 0;
	}

	bool StringUtility::Contains(StringView Text, StringView Substring, SearchMode Mode)
	{
		StringView Result = Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Contains, Mode, 0, nullptr);
		return !Result.IsEmpty();
	}

	StringView StringUtility::Common(StringView Text1, StringView Text2)
	{
		uint64 Count = StringCApi::Common(Text1.C(), Text2.C());
		return StringView(Text1.C(), Count);
	}

	StringView StringUtility::Find(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::FindAll(StringView Text, StringView Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Find, Mode, 0, &Results);
		return Results;
	}

	StringView StringUtility::Split(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::SplitAll(StringView Text, StringView Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Split, Mode, 0, &Results);
		return Results;
	}

	int64 StringUtility::ToInteger(StringView Text, int32 Radix)
	{
		return StringCApi::ToInteger(Text.C());
	}

	uint64 StringUtility::ToUnsignedInteger(StringView Text, int32 Radix)
	{
		return StringCApi::ToUnsignedInteger(Text.C());
	}

	double StringUtility::ToDouble(StringView Text)
	{
		return StringCApi::ToDouble(Text.C());
	}

	String StringUtility::ToStringI(int64 Number, StringView Format)
	{
		return StringUtility::Format(Format, Number);
	}

	String StringUtility::ToStringU(uint64 Number, StringView Format)
	{
		return StringUtility::Format(Format, Number);
	}

	String StringUtility::ToStringF(float Number, StringView Format)
	{
		return StringUtility::Format(Format, Number);
	}

	String StringUtility::ToStringD(double Number, StringView Format)
	{
		return StringUtility::Format(Format, Number);
	}

	String StringUtility::ToStringB(bool State, StringView Format)
	{
		return StringUtility::Format(Format, State ? "True" : "False");
	}

	StringView StringUtility::Search(const char* Text, const char* Substring, uint64 Size, SearchBehaviour Behaviour, SearchMode Mode, uint64 Offset, List<StringView>* Results)
	{
		uint64 Index = 0;
		const char* Previous = Text;
		const char* Pointer = Text;
		StringView Result;

		do
		{
			switch (Mode)
			{
			case SearchMode::Substring: Pointer = StringCApi::SearchStr(Pointer, Substring, Size); break;
			case SearchMode::Characters: Pointer = StringCApi::SearchChr(Pointer, Substring); break;
			}

			switch (Behaviour)
			{
			case SearchBehaviour::Contains:
			case SearchBehaviour::Find: Result = Pointer != nullptr ? StringView(Pointer) : StringView(); break;
			case SearchBehaviour::Split: Result = Pointer != nullptr ? StringView(Previous, StringCApi::Length(Previous) - StringCApi::Length(Pointer)) : StringView(Previous); break;
			}

			if (Results && Result != String::Empty)
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
			++Pointer;
			Previous = Pointer;
		} while (true);

		return Result;
	}

	//-----------------------------------------------------------------------------------------------------------------------
	// Operator
	//-----------------------------------------------------------------------------------------------------------------------

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
		Return.Remove(TextB, 0, 0, true);
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
}
