#include "Core/NexusFrameworkPch.h"
#include "StringView.h"

namespace NxFr
{
	StringView::StringView()
		:Data(""), Count(0)
	{
	}

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

	bool StringView::Start(StringView Substring) const
	{
		return StringUtility::Start(C(), Substring);
	}

	bool StringView::End(StringView Substring) const
	{
		return StringUtility::End(C(), Substring);
	}

	bool StringView::Contains(StringView Substring) const
	{
		return StringUtility::Contains(C(), Substring);
	}

	StringView StringView::Find(StringView Substring, uint64 Offset) const
	{
		return StringUtility::Find(C(), Substring, Offset);
	}

	List<StringView> StringView::FindAll(StringView Substring) const
	{
		return StringUtility::FindAll(C(), Substring);
	}

	StringView StringView::Split(StringView Substring, uint64 Offset) const
	{
		return StringUtility::Split(C(), Substring, Offset);
	}

	List<StringView> StringView::SplitAll(StringView Substring) const
	{
		return StringUtility::SplitAll(C(), Substring);
	}

	String StringView::ToString() const
	{
		return String(C(), Count);
	}

	StringView StringView::ToView(uint64 Offset, uint64 Size) const
	{
		NEXUS_ASSERT(Offset + Size <= Count, Default, "Invalid String view");
		return StringView(Data + Offset, Size);
	}
}
