#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringId.h"

namespace NxFr
{
	static Dictionary<GUID, String>& GetStringsTable() { static Dictionary<GUID, String> StringsTable(97, nullptr); return StringsTable; }

	GUID StringId::InternString(StringView Text)
	{
		Dictionary<GUID, String>& StringsTable = GetStringsTable();

		GUID Id = !Text.IsEmpty() ? Hash<>::HashObject(Text) : 0;
		if (!StringsTable.ContainsKey(Id))
		{
			StringsTable.Append(Id, Text);
		}
		return Id;
	}

	const String& StringId::LookupString(GUID Id)
	{
		Dictionary<GUID, String>& StringsTable = GetStringsTable();
		String* Value = StringsTable.TryGet(Id);
		return Value != nullptr ? *Value : StringId::Unknown;
	}

	StringId::StringId()
		: Id(0)
	{
	}

	StringId::StringId(GUID Id)
		: Id(Id)
	{
	}

	StringId::StringId(StringView Text)
		: Id(InternString(Text))
	{
	}

	StringId::operator GUID() const
	{
		return Id;
	}

	StringId::operator StringView() const
	{
		return LookupString(Id);
	}

	bool StringId::operator==(GUID Other) const
	{
		return Id == Other;
	}

	bool StringId::operator!=(GUID Other) const
	{
		return !(*this == Other);
	}

	bool StringId::operator==(const StringId& Other) const
	{
		return Id == Other.Id;
	}

	bool StringId::operator!=(const StringId& Other) const
	{
		return !(*this == Other);
	}

	bool StringId::operator<(const StringId& Other) const
	{
		return LookupString(Id) < LookupString(Other.Id);
	}

	bool StringId::operator<=(const StringId& Other) const
	{
		return LookupString(Id) <= LookupString(Other.Id);
	}

	bool StringId::operator>(const StringId& Other) const
	{
		return LookupString(Id) > LookupString(Other.Id);
	}

	bool StringId::operator>=(const StringId& Other) const
	{
		return LookupString(Id) >= LookupString(Other.Id);
	}

	GUID StringId::GetId() const
	{
		return Id;
	}

	const String& StringId::GetString() const
	{
		return LookupString(Id);
	}

	StringId operator""_Sid(const char* Text, uint64 Size)
	{
		return StringId(Text);
	}
}
