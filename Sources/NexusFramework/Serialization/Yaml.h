#pragma once

#include "NexusFramework/External/YamlCpp.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	namespace Yaml
	{
		inline constexpr uint64 SmallSequence = 10;

		NX_FRAMEWORK_API String Serialize(const YAML::Node& Data);
		NX_FRAMEWORK_API String Serialize(const YAML::Emitter& Data);
		NX_FRAMEWORK_API void SerializeFile(const YAML::Node& Data, StringView Path);
		NX_FRAMEWORK_API void SerializeFile(const YAML::Emitter& Data, StringView Path);
		NX_FRAMEWORK_API YAML::Node Deserialize(StringView Data);
		NX_FRAMEWORK_API YAML::Node DeserializeFile(StringView Path);

		NX_FRAMEWORK_API YAML::Node EmitterToNode(const YAML::Emitter& Data);
	}
}
