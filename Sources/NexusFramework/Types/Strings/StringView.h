#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"

namespace NxFr
{
	template <typename T> class List;

	struct StringView
	{
	public:
		NEXUS_FRAMEWORK_API StringView();
		NEXUS_FRAMEWORK_API StringView(const char* Text);
		NEXUS_FRAMEWORK_API StringView(const char* Text, uint64 Size);
		NEXUS_FRAMEWORK_API StringView(const char* Text, uint64 Offset, uint64 Size);
		NEXUS_FRAMEWORK_API StringView(const String& Text);

		NEXUS_FRAMEWORK_API const Iterator::StringCharacter Begin() const;
		NEXUS_FRAMEWORK_API const Iterator::StringToken Begin(StringView Token) const;
		NEXUS_FRAMEWORK_API const Iterator::StringCharacter End() const;
		NEXUS_FRAMEWORK_API const Iterator::StringToken End(StringView Token) const;

		NEXUS_FRAMEWORK_API StringView Substring(uint64 Offset, uint64 Size) const;
		NEXUS_FRAMEWORK_API const char* CString() const;

		NEXUS_FRAMEWORK_API const char* C(bool Exact = false) const { return Exact ? CString() : Data; }
		NEXUS_FRAMEWORK_API bool IsEmpty() const { return Count == 0; }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Count; }

	private:
		const char* Data;
		uint64 Count;
	};
}
