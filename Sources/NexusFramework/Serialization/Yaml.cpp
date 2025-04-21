#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	namespace Yaml
	{
		String Serialize(const YAML::Node& Data)
		{
			YAML::Emitter Emitter;
			Emitter << Data;

			return Serialize(Emitter);
		}

		String Serialize(const YAML::Emitter& Data)
		{
			return Data.c_str();
		}

		void SerializeFile(const YAML::Node& Data, StringView Path)
		{
			YAML::Emitter Emitter;
			Emitter << Data;

			SerializeFile(Emitter, Path);
		}

		void SerializeFile(const YAML::Emitter& Data, StringView Path)
		{
			File F = File(Path);
			F.Create();
			F.Open(File::Mode::Write);
			F.WriteText(Serialize(Data));
			F.Close();
		}

		YAML::Node Deserialize(StringView Data)
		{
			return YAML::Load(Data.C());
		}

		YAML::Node DeserializeFile(StringView Path)
		{
			return YAML::LoadFile(std::string(Path.C()));
		}
	}
}

namespace YAML
{
	Node convert<NxFr::String>::encode(const NxFr::String& rhs)
	{
		return Node(rhs.C());
	}

	bool convert<NxFr::String>::decode(const Node& node, NxFr::String& rhs)
	{
		rhs = node.as<std::string>().c_str();
		return true;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::String& rhs)
	{
		out << rhs.C();
		return out;
	}

	Node convert<NxFr::StringView>::encode(const NxFr::StringView& rhs)
	{
		return Node(rhs.C());
	}

	bool convert<NxFr::StringView>::decode(const Node& node, NxFr::StringView& rhs)
	{
		rhs = node.as<std::string>().c_str();
		return true;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringView& rhs)
	{
		out << rhs.C();
		return out;
	}

	Node convert<NxFr::StringId>::encode(const NxFr::StringId& rhs)
	{
		return Node(rhs.C());
	}

	bool convert<NxFr::StringId>::decode(const Node& node, NxFr::StringId& rhs)
	{
		if (!node.IsScalar())
		{
			return false;
		}

		rhs = NxFr::StringId(node.as<NxFr::StringView>());
		return true;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringId& rhs)
	{
		out << rhs.C();
		return out;
	}
}
