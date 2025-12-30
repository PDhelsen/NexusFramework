#pragma once

#include "NexusFramework/External/YamlCpp.h"

#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	namespace Yaml
	{
		const uint64 SmallSequence = 10;

		NEXUS_FRAMEWORK_API String Serialize(const YAML::Node& Data);
		NEXUS_FRAMEWORK_API String Serialize(const YAML::Emitter& Data);
		NEXUS_FRAMEWORK_API void SerializeFile(const YAML::Node& Data, StringView Path);
		NEXUS_FRAMEWORK_API void SerializeFile(const YAML::Emitter& Data, StringView Path);
		NEXUS_FRAMEWORK_API YAML::Node Deserialize(StringView Data);
		NEXUS_FRAMEWORK_API YAML::Node DeserializeFile(StringView Path);

		NEXUS_FRAMEWORK_API YAML::Node EmitterToNode(const YAML::Emitter& Data);
	}
}
