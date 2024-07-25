#include "Core/NexusEnginePch.h"
#include "StringView.h"

namespace NxEn
{
	StringView::StringView(const char* Text)
		:Data(Text), Count(StringCApi::Length(Text))
	{
	}

	StringView::StringView(const char* Text, uint64 Size)
		:Data(Text), Count(Size)
	{
	}

	StringView::StringView(const String& Text)
		:Data(Text.C()), Count(Text.GetCount())
	{
	}

	bool StringView::Start(const StringView& Substring) const
	{
		return StringUtility::Start(C(), Substring);
	}

	bool StringView::End(const StringView& Substring) const
	{
		return StringUtility::End(C(), Substring);
	}

	bool StringView::Contains(const StringView& Substring) const
	{
		return StringUtility::Contains(C(), Substring);
	}

	StringView StringView::Find(const StringView& Substring, uint64 Offset) const
	{
		return StringUtility::Find(C(), Substring, Offset);
	}

	List<StringView> StringView::FindAll(const StringView& Substring) const
	{
		return Move(StringUtility::FindAll(C(), Substring));
	}

	StringView StringView::Split(const StringView& Substring, uint64 Offset) const
	{
		return StringUtility::Split(C(), Substring, Offset);
	}

	List<StringView> StringView::SplitAll(const StringView& Substring) const
	{
		return Move(StringUtility::SplitAll(C(), Substring));
	}

	String StringView::ToString() const
	{
		return Move(String(C(), Count));
	}

	StringView StringView::ToView(uint64 Offset, uint64 Size) const
	{
		NEXUS_ASSERT(Offset + Size <= Count, "Invalid String view");
		return StringView(Data + Offset, Size);
	}
}
