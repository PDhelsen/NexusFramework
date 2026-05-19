#include "NexusTests/Core/NexusTestsDummy.h"

namespace NxTs
{
	NxFr::String Dummy::GeneratePath(NxFr::StringView Path)
	{
		return NxFr::Path::Combine(NxFr::Globals::Paths::Temp, Path);
	}

	NxFr::StringId Dummy::GetId()
	{
		return "Dummy"_Sid;
	}

	NxFr::StringView Dummy::GetText()
	{
		return "This is a dummy text string.";
	}

	NxFr::Byte* Dummy::GetData()
	{
		static NxFr::Byte Data[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
		return Data;
	}

	Dummy Dummy::GetReference()
	{
		return Dummy(55, "Nexus UnitTest");
	}

	void Dummy::DummyFunction1(uint64 A)
	{
	}

	uint64 Dummy::DummyFunction2(uint64 A)
	{
		return A;
	}

	Dummy::Dummy()
		: Key(0), Value("")
	{
	}

	Dummy::Dummy(uint64 Key)
		: Key(Key), Value("")
	{
	}

	Dummy::Dummy(NxFr::StringView Value)
		: Key(0), Value(Value)
	{
	}
	Dummy::Dummy(uint64 Key, NxFr::StringView Value)
		: Key(Key), Value(Value)
	{
	}
	Dummy::Dummy(const Dummy& Other)
		: Key(Other.Key), Value(Other.Value)
	{
	}
	Dummy::Dummy(Dummy&& Other) noexcept
		: Key(NxFr::Move(Other.Key)), Value(NxFr::Move(Other.Value))
	{
	}
	Dummy::~Dummy()
	{
		NEXUS_ASSERT(Key != NxFr::Integer::MaxUI64, Default, "Dummy already destroyed");
		Key = NxFr::Integer::MaxUI64;
	}
	Dummy& Dummy::operator=(const Dummy& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Key = Other.Key;
		Value = Other.Value;

		return *this;
	}
	Dummy& Dummy::operator=(Dummy&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Key = NxFr::Move(Other.Key);
		Value = NxFr::Move(Other.Value);

		return *this;
	}
	bool Dummy::operator==(const Dummy& Other) const
	{
		return Key == Other.Key;
	}
	bool Dummy::operator!=(const Dummy& Other) const
	{
		return !(*this == Other);
	}
	bool Dummy::operator>(const Dummy& Other) const
	{
		return Key > Other.Key;
	}
	bool Dummy::operator<(const Dummy& Other) const
	{
		return Key < Other.Key;
	}
	bool Dummy::operator>=(const Dummy& Other) const
	{
		return Key >= Other.Key;
	}
	bool Dummy::operator<=(const Dummy& Other) const
	{
		return Key <= Other.Key;
	}

	uint64 Dummy::operator()(uint64 Value)
	{
		return Key + Value;
	}

	uint64 Dummy::operator()(uint64 Value) const
	{
		return Key + Value;
	}

	void Dummy::Function1(uint64 Value)
	{
	}

	void Dummy::Function1(uint64 Value) const
	{
	}

	uint64 Dummy::Function2(uint64 Value)
	{
		return Key + Value;
	}

	uint64 Dummy::Function2(uint64 Value) const
	{
		return Key + Value;
	}
}

namespace YAML
{
	Node convert<NxTs::Dummy>::encode(const NxTs::Dummy& rhs)
	{
		Node node;
		node["key"] = rhs.Key;
		node["value"] = rhs.Value;
		return node;
	}

	bool convert<NxTs::Dummy>::decode(const Node& node, NxTs::Dummy& rhs)
	{
		rhs.Key = node["key"].as<uint64>();
		rhs.Value = node["value"].as<NxFr::String>();
		return true;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const NxTs::Dummy& rhs)
	{
		out << YAML::Key << "key" << YAML::Value << rhs.Key;
		out << YAML::Key << "value" << YAML::Value << rhs.Value;
		return out;
	}
}
