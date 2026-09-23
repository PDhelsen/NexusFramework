#pragma once

#include "NexusFramework/External/YamlCpp.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	namespace Yaml
	{
		inline constexpr uint64 SmallSequence = 10;

		NX_FRAMEWORK_API String Serialize(const Node& Data);
		NX_FRAMEWORK_API void SerializeAndSave(const Node& Data, StringView Path);
		NX_FRAMEWORK_API Node Deserialize(StringView Data);
		NX_FRAMEWORK_API Node LoadAndDeserialize(StringView Path);
	}
}

namespace YAML
{
	template<>
	struct convert<NxFr::Yaml::Node>
	{
		static Node encode(const NxFr::Yaml::Node& rhs)
		{
			return Node(rhs.Data);
		}

		static bool decode(const Node& node, NxFr::Yaml::Node& rhs)
		{
			rhs = node;
			return true;
		}
	};
}
