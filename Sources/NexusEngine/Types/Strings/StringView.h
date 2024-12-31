#pragma once

#include "Core/NexusEngineCore.h"
#include "Types/Numbers/Integer.h"
#include "Types/Strings/String.h"

namespace NxEn
{
	template <typename T> class List;

	struct StringView
	{
	public:
		NEXUS_ENGINE_API StringView();
		NEXUS_ENGINE_API StringView(const char* Text);
		NEXUS_ENGINE_API StringView(const char* Text, uint64 Size);
		NEXUS_ENGINE_API StringView(const String& Text);

		NEXUS_ENGINE_API bool Start(StringView Substring) const;
		NEXUS_ENGINE_API bool End(StringView Substring) const;
		NEXUS_ENGINE_API bool Contains(StringView Substring) const;
		NEXUS_ENGINE_API StringView Find(StringView Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> FindAll(StringView Substring) const;
		NEXUS_ENGINE_API StringView Split(StringView Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> SplitAll(StringView Substring) const;

		NEXUS_ENGINE_API String ToString() const;
		NEXUS_ENGINE_API StringView ToView(uint64 Offset, uint64 Size) const;

		NEXUS_ENGINE_API const char* C() const { return Data; }
		NEXUS_ENGINE_API bool IsEmpty() const { return Count == 0; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Count; }

	private:
		const char* Data;
		uint64 Count;
	};
}
