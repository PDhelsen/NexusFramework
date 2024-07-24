#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/String.h"

namespace NxEn
{
	struct StringView
	{
	public:
		NEXUS_ENGINE_API StringView(const char* Text);
		NEXUS_ENGINE_API StringView(const char* Text, uint64 Size);
		NEXUS_ENGINE_API StringView(const String& Text);

		NEXUS_ENGINE_API bool Start(const StringView& Substring) const;
		NEXUS_ENGINE_API bool End(const StringView& Substring) const;
		NEXUS_ENGINE_API bool Contains(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Find(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> FindAll(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Split(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> SplitAll(const StringView& Substring) const;

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
