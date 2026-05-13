#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Strings/StringId.h"

#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	static Mutex& GetLock() { static Mutex Guard; return Guard; }
	static Dictionary<GUID, StringView>& GetIds() { static Dictionary<GUID, StringView> Ids(97, nullptr); return Ids; }
	static Dequeue<String>& GetStrings() { static Dequeue<String> Strings(nullptr); return Strings; }

	GUID StringId::InternString(StringView Text)
	{
		Mutex* Guard = Globals::PlatformTarget ? &GetLock() : nullptr;
		if (Guard)
		{
			Guard->Lock();
		}

		Dictionary<GUID, StringView>& Ids = GetIds();
		GUID Id = Hash<>::HashObject(Text);
		if (Ids.TryGet(Id) == nullptr)
		{
			Dequeue<String>& Instances = GetStrings();
			String& Instance = Instances.AppendBack(Text);
			Ids.Append(Id, Instance);
		}

		if (Guard)
		{
			Guard->Unlock();
		}

		return Id;
	}

	StringView StringId::LookupString(GUID Id)
	{
		Mutex* Guard = Globals::PlatformTarget ? &GetLock() : nullptr;
		if (Guard)
		{
			Guard->Lock();
		}

		Dictionary<GUID, StringView>& Ids = GetIds();
		StringView* Value = Ids.TryGet(Id);
		StringView Result = Value != nullptr ? *Value : StringView(StringUtility::Unknown);

		if (Guard)
		{
			Guard->Unlock();
		}

		return Result;
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
