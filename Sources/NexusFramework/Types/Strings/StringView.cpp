#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringView.h"

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
		return StringUtility::Start(*this, Substring);
	}

	bool StringView::End(StringView Substring) const
	{
		return StringUtility::End(*this, Substring);
	}

	bool StringView::Contains(StringView Substring) const
	{
		return StringUtility::Contains(*this, Substring);
	}

	StringView StringView::Find(StringView Substring, uint64 Offset) const
	{
		return StringUtility::Find(*this, Substring, Offset);
	}

	List<StringView> StringView::FindAll(StringView Substring) const
	{
		return StringUtility::FindAll(*this, Substring);
	}

	StringView StringView::Split(StringView Substring, uint64 Offset) const
	{
		return StringUtility::Split(*this, Substring, Offset);
	}

	List<StringView> StringView::SplitAll(StringView Substring) const
	{
		return StringUtility::SplitAll(*this, Substring);
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
