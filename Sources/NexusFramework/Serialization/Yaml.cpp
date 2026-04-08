#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Yaml.h"
#include "NexusFramework/IO/Stream.h"

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
			TextStream Stream(Path);
			Stream.Open(File::Mode::Write);
			Stream.WriteAll(Serialize(Data));
			Stream.Close();
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
