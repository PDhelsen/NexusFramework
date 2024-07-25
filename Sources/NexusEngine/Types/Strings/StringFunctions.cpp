#include "Core/NexusEnginePch.h"
#include "StringFunctions.h"

namespace NxEn
{
	bool StringUtility::Start(StringView Text, StringView Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring.C(), Substring.GetCount());
		return Result && Text.C() == Result;
	}

	bool StringUtility::End(StringView Text, StringView Substring)
	{
		const char* Result = StringCApi::SearchStr(Text.C(), Substring.C(), Substring.GetCount());
		return Result && Substring.GetCount() == StringCApi::Length(Result);
	}

	bool StringUtility::Contains(StringView Text, StringView Substring, SearchMode Mode)
	{
		StringView Result = Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Contains, Mode, 0, nullptr);
		return !Result.IsEmpty();
	}

	StringView StringUtility::Find(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Find, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::FindAll(StringView Text, StringView Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Find, Mode, 0, &Results);
		return Move(Results);
	}

	StringView StringUtility::Split(StringView Text, StringView Substring, uint64 Offset, SearchMode Mode)
	{
		return Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Split, Mode, Offset, nullptr);
	}

	List<StringView> StringUtility::SplitAll(StringView Text, StringView Substring, SearchMode Mode)
	{
		List<StringView> Results;
		Search(Text.C(), Substring.C(), Substring.GetCount(), SearchBehaviour::Split, Mode, 0, &Results);
		return Move(Results);
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

	String operator+(StringView TextA, StringView TextB)
	{
		String Return = String(TextA.GetCount() + TextB.GetCount());
		Return.Append(TextA);
		Return.Append(TextB);
		return Move(Return);
	}

	String operator-(StringView TextA, StringView TextB)
	{
		String Return = String(TextA.GetCount());
		Return.Append(TextA);
		Return.Remove(TextB, 0, 0, true);
		return Move(Return);
	}

	bool operator==(StringView TextA, StringView TextB)
	{
		return TextA.GetCount() == TextB.GetCount() && StringCApi::Compare(TextA.C(), TextB.C()) == 0;
	}

	bool operator!=(StringView TextA, StringView TextB)
	{
		return !(TextA == TextB);
	}

	bool operator>(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() > TextB.GetCount();
		}
		return Comparaison > 0;
	}

	bool operator>=(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() >= TextB.GetCount();
		}
		return Comparaison >= 0;
	}

	bool operator<(StringView TextA, StringView TextB)
	{
		int8 Comparaison = StringCApi::Compare(TextA.C(), TextB.C());
		if (Comparaison == 0 && TextA.GetCount() != TextB.GetCount())
		{
			return TextA.GetCount() < TextB.GetCount();
		}
		return Comparaison < 0;
	}

	bool operator<=(StringView TextA, StringView TextB)
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
