#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	namespace Yaml
	{
		Node Load(NxFr::StringView Data)
		{
			return YAML::Load(Data.C());
		}

		Node LoadFile(NxFr::StringView Path)
		{
			return YAML::LoadFile(std::string(Path.C()));
		}
	}
}
