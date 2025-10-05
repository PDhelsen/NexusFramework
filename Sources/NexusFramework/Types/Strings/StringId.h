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

		NEXUS_FRAMEWORK_API static GUID InternString(StringView Text);
		NEXUS_FRAMEWORK_API static const String& LookupString(GUID Id);

		NEXUS_FRAMEWORK_API StringId();
		NEXUS_FRAMEWORK_API StringId(GUID Id);
		NEXUS_FRAMEWORK_API StringId(StringView Text);

		NEXUS_FRAMEWORK_API operator GUID() const;
		NEXUS_FRAMEWORK_API operator StringView() const;

		NEXUS_FRAMEWORK_API bool operator==(GUID Other) const;
		NEXUS_FRAMEWORK_API bool operator!=(GUID Other) const;
		NEXUS_FRAMEWORK_API bool operator==(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator!=(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator<(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator<=(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator>(const StringId& Other) const;
		NEXUS_FRAMEWORK_API bool operator>=(const StringId& Other) const;

		NEXUS_FRAMEWORK_API GUID GetId() const;
		NEXUS_FRAMEWORK_API const String& GetString() const;

		NEXUS_FRAMEWORK_API const char* C() const { return LookupString(Id).C(); }
		NEXUS_FRAMEWORK_API bool IsValid() const { return Id == 0; }
		NEXUS_FRAMEWORK_API bool IsEmpty() const { return LookupString(Id).IsEmpty(); }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return LookupString(Id).GetCount(); }

	private:
		GUID Id;
	};

	NEXUS_FRAMEWORK_API StringId operator""_Sid(const char* Text, uint64 Size);
}

using NxFr::operator""_Sid;
