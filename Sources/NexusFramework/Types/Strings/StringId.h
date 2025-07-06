#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	struct StringId
	{
	public:
		inline static const String Unknown = "Unknown";

		NEXUS_FRAMEWORK_API StringId();
		NEXUS_FRAMEWORK_API StringId(GUID Id);
		NEXUS_FRAMEWORK_API explicit StringId(StringView Text);

		NEXUS_FRAMEWORK_API bool operator==(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator==(GUID Other) const;
		NEXUS_FRAMEWORK_API bool operator!=(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator!=(GUID Other) const;
		NEXUS_FRAMEWORK_API operator GUID() const;

		NEXUS_FRAMEWORK_API const String& ToString() const;
		NEXUS_FRAMEWORK_API const char* C() const;
		NEXUS_FRAMEWORK_API const GUID GetId() const;

	private:
		GUID Id;
	};

	NEXUS_FRAMEWORK_API StringId operator""_Sid(const char* Text, uint64 Size);
}
using NxFr::operator""_Sid;
