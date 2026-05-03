#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	static Mutex& GetLock() { static Mutex Guard; return Guard; }
	static Dictionary<GUID, StringView>& GetStringsTable() { static Dictionary<GUID, StringView> StringsTable(97, nullptr); return StringsTable; }
	static Dequeue<String>& GetStrings() { static Dequeue<String> Strings(nullptr); return Strings; }

	GUID StringId::InternString(StringView Text)
	{
		Lock Guard(GetLock());

		Dictionary<GUID, StringView>& Table = GetStringsTable();
		GUID Id = Hash<>::HashObject(Text);
		if (Table.TryGet(Id) == nullptr)
		{
			Dequeue<String>& Instances = GetStrings();
			String& Instance = Instances.AppendBack(Text);
			Table.Append(Id, Instance);
		}
		return Id;
	}

	StringView StringId::LookupString(GUID Id)
	{
		Lock Guard(GetLock());

		Dictionary<GUID, StringView>& Table = GetStringsTable();
		StringView* Value = Table.TryGet(Id);
		return Value != nullptr ? *Value : StringView(StringUtility::Unknown);
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

	StringView StringId::GetString() const
	{
		return LookupString(Id);
	}

	StringId operator""_Sid(const char* Text, uint64 Size)
	{
		return StringId(Text);
	}
}
