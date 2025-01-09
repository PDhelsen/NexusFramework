#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Types/Numbers/Integer.h"
#include "Misc/Hash.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxEn
{
	struct StringId
	{
	public:
		NEXUS_FRAMEWORK_API explicit StringId();
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
using NxEn::operator""_Sid;
