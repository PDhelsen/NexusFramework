#include "Core/NexusFrameworkPch.h"
#include "StringId.h"

namespace NxEn
{
	static Dictionary<GUID, String>& GetStringsTable() { static Dictionary<GUID, String> StringsTable(97, nullptr); return StringsTable; }

	static GUID InternString(StringView Text)
	{
		Dictionary<GUID, String>& StringsTable = GetStringsTable();

		GUID Id = Hash<>::HashObject(Text);
		if (!StringsTable.ContainsKey(Id))
		{
			StringsTable.Append(Move(Id), Move(Text.ToString()));
		}
		return Id;
	}

	StringId::StringId()
		: Id(0)
	{
	}

	StringId::StringId(StringView Text)
		: Id(InternString(Text))
	{
	}

	bool StringId::operator==(const StringId& Other) const
	{
		return Id == Other.Id;
	}

	bool StringId::operator==(GUID Other) const
	{
		return Id == Other;
	}

	bool StringId::operator!=(const StringId& Other) const
	{
		return !(*this == Other);
	}

	bool StringId::operator!=(GUID Other) const
	{
		return !(*this == Other);
	}

	StringId::operator GUID() const
	{
		return Id;
	}

	const String& StringId::ToString() const
	{
		Dictionary<GUID, String>& StringsTable = GetStringsTable();
		return StringsTable[Id];
	}

	const char* StringId::C() const
	{
		Dictionary<GUID, String>& StringsTable = GetStringsTable();
		return StringsTable[Id].C();
	}

	const GUID StringId::GetId() const
	{
		return Id;
	}

	StringId operator""_Sid(const char* Text, uint64 Size)
	{
		return StringId(StringView(Text, Size));
	}
}
