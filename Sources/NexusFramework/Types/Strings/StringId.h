#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	struct NX_FRAMEWORK_API StringId
	{
	public:
		static GUID InternString(StringView Text);
		static StringView LookupString(GUID Id);

		StringId();
		StringId(GUID Id);
		StringId(StringView Text);

		operator GUID() const;
		operator StringView() const;

		bool operator==(GUID Other) const;
		bool operator!=(GUID Other) const;
		bool operator==(const StringId& Other) const;
		bool operator!=(const StringId& Other) const;
		bool operator<(const StringId& Other) const;
		bool operator<=(const StringId& Other) const;
		bool operator>(const StringId& Other) const;
		bool operator>=(const StringId& Other) const;

		GUID GetId() const;
		StringView GetString() const;

		const char* C() const { return LookupString(Id).C(); }
		bool IsValid() const { return Id != 0; }
		bool IsEmpty() const { return LookupString(Id).IsEmpty(); }
		uint64 GetCount() const { return LookupString(Id).GetCount(); }

	private:
		GUID Id;
	};

	NX_FRAMEWORK_API StringId operator""_Sid(const char* Text, uint64 Size);
}

using NxFr::operator""_Sid;
