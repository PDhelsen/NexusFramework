#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	static String& GetLocalBuffer() { static thread_local String LocalBuffer(64, nullptr); return LocalBuffer; }

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

	const Iterator::StringCharacter StringView::Begin() const
	{
		return Iterator::StringCharacter(*this, 0);
	}

	const Iterator::StringToken StringView::Begin(StringView Token) const
	{
		return Iterator::StringToken(Token, *this, 0);
	}

	const Iterator::StringCharacter StringView::End() const
	{
		return Iterator::StringCharacter(*this, GetCount());
	}

	const Iterator::StringToken StringView::End(StringView Token) const
	{
		return Iterator::StringToken(Token, *this, GetCount());
	}

	StringView StringView::Substring(uint64 Offset, uint64 Size) const
	{
		return StringView(C(), Offset, Size);
	}

	const char* StringView::CString() const
	{
		String& Buffer = GetLocalBuffer();
		Buffer.Clear();
		Buffer += *this;
		return Buffer.C();
	}
}
