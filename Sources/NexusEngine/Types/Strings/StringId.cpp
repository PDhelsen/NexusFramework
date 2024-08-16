#include "Core/NexusEnginePch.h"
#include "StringId.h"

namespace NxEn
{
	Dictionary<GUID, String> StringId::Tables = Dictionary<GUID, String>();

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
		return Tables[Id];
	}

	const GUID StringId::GetId() const
	{
		return Id;
	}

	GUID StringId::InternString(StringView Text)
	{
		GUID Id = Hash<StringView>::HashObject(Text);
		if (!Tables.ContainsKey(Id))
		{
			Tables.Append(Move(Id), Move(Text.ToString()));
		}
		return Id;
	}
}
