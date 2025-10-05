#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	static String Buffer;

	StringView::StringView()
		: Data(""), Count(0)
	{
	}

	StringView::StringView(const char* Text)
		: Data(Text), Count(StringCApi::Length(Text))
	{
	}

	StringView::StringView(const char* Text, uint64 Size)
		: Data(Text), Count(Size)
	{
	}

	StringView::StringView(const char* Text, uint64 Offset, uint64 Size)
		: Data(Text + Offset), Count(Size)
	{
		NEXUS_ASSERT(Offset + Size <= StringCApi::Length(Text), Default, "Invalid String view");
	}

	StringView::StringView(const String& Text)
		:Data(Text.C()), Count(Text.GetCount())
	{
	}

	StringView StringView::Substring(uint64 Offset, uint64 Size) const
	{
		return StringView(C(), Offset, Size);
	}

	const char* StringView::CString() const
	{
		Buffer.Clear();
		Buffer += *this;
		return Buffer.C();
	}
}
