#pragma once

#include "Core/NexusEngine.h"
#include "Misc/Hash.h"
#include "Types/Integer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxEn
{
	template <typename K, typename T, class H, float LF> class Dictionary;

	struct StringId
	{
	public:
		NEXUS_ENGINE_API explicit StringId(const StringView& Text);

		NEXUS_ENGINE_API bool operator==(const StringId& Other) const;
		NEXUS_ENGINE_API bool operator==(GUID Other) const;
		NEXUS_ENGINE_API bool operator!=(const StringId& Other) const;
		NEXUS_ENGINE_API bool operator!=(GUID Other) const;
		NEXUS_ENGINE_API operator GUID() const;

		NEXUS_ENGINE_API const String& ToString() const;
		NEXUS_ENGINE_API const GUID GetId() const;

	private:
		static GUID InternString(const StringView& Text);

		static Dictionary<GUID, String, Fnv1a64, 1.0f> Tables;

		const GUID Id;
	};
}
