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
		NEXUS_FRAMEWORK_API StringView(const String& Text);

		NEXUS_FRAMEWORK_API bool Start(StringView Substring) const;
		NEXUS_FRAMEWORK_API bool End(StringView Substring) const;
		NEXUS_FRAMEWORK_API bool Contains(StringView Substring) const;
		NEXUS_FRAMEWORK_API StringView Find(StringView Substring, uint64 Offset = 0) const;
		NEXUS_FRAMEWORK_API List<StringView> FindAll(StringView Substring) const;
		NEXUS_FRAMEWORK_API StringView Split(StringView Substring, uint64 Offset = 0) const;
		NEXUS_FRAMEWORK_API List<StringView> SplitAll(StringView Substring) const;

		NEXUS_FRAMEWORK_API String ToString() const;
		NEXUS_FRAMEWORK_API StringView ToView(uint64 Offset, uint64 Size) const;

		NEXUS_FRAMEWORK_API const char* C() const { return Data; }
		NEXUS_FRAMEWORK_API bool IsEmpty() const { return Count == 0; }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Count; }

	private:
		const char* Data;
		uint64 Count;
	};
}
