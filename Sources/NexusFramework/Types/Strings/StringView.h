#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"

namespace NxFr
{
	struct NX_FRAMEWORK_API StringView
	{
	public:
		static StringView GetTempExactString(StringView Substring);

		StringView();
		StringView(const char* Text);
		StringView(const char* Text, uint64 Size);
		StringView(const char* Text, uint64 Offset, uint64 Size);
		StringView(const String& Text);

		const Iterator::StringCharacter Begin() const;
		const Iterator::StringToken Begin(StringView Token) const;
		const Iterator::StringCharacter End() const;
		const Iterator::StringToken End(StringView Token) const;

		StringView Substring(uint64 Offset, uint64 Size) const;
		String ExactString() const;

		const char* C(bool Exact = false) const { return Exact ? GetTempExactString(*this).Data : Data; }
		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }

	private:
		const char* Data;
		uint64 Count;
	};
}
