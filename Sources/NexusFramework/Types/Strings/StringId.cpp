#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringId.h"

namespace NxFr
{
	static Dictionary<GUID, String>& GetStringsTable() { static Dictionary<GUID, String> StringsTable(97, nullptr); return StringsTable; }

	static GUID InternString(StringView Text)
	{
		Dictionary<GUID, String>& StringsTable = GetStringsTable();

		GUID Id = !Text.IsEmpty() ? Hash<>::HashObject(Text) : 0;
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

	StringId::StringId(GUID Id)
		: Id(Id)
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
		String* Value = StringsTable.TryGet(Id);
		return Value != nullptr ? *Value : Unknown;
	}

	const char* StringId::C() const
	{
		return ToString().C();
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
