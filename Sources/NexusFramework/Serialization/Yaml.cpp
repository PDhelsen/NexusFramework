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
			F.WriteText(Data.c_str());
			F.Close();
		}

		YAML::Node Deserialize(StringView Data)
		{
			return YAML::Load(Data.C());
		}

		YAML::Node DeserializeFile(StringView Path)
		{
			return YAML::LoadFile(Path.C());
		}

		YAML::Node EmitterToNode(const YAML::Emitter& Data)
		{
			return YAML::Load(Data.c_str());
		}
	}
}
