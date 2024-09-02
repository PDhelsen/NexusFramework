#pragma once

#include "Core/NexusEngine.h"
#include "Misc/Hash.h"
#include "Types/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxEn
{
	template <typename K, typename T, class H> class Dictionary;

	struct StringId
	{
	public:
		NEXUS_ENGINE_API explicit StringId(StringView Text);

		NEXUS_ENGINE_API bool operator==(const StringId& Other) const;
		NEXUS_ENGINE_API bool operator==(GUID Other) const;
		NEXUS_ENGINE_API bool operator!=(const StringId& Other) const;
		NEXUS_ENGINE_API bool operator!=(GUID Other) const;
		NEXUS_ENGINE_API operator GUID() const;

		NEXUS_ENGINE_API const String& ToString() const;
		NEXUS_ENGINE_API const char* C() const;
		NEXUS_ENGINE_API const GUID GetId() const;

	private:
		static GUID InternString(StringView Text);

		static Dictionary<GUID, String, Fnv1a64> Tables;

		GUID Id;
	};

	NEXUS_ENGINE_API StringId operator""_Sid(const char* Text, uint64 Size);
}
using NxEn::operator""_Sid;
