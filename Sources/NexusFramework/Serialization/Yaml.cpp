#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace YAML
{
	Node Load(NxFr::StringView Data)
	{
		return Load(Data.C());
	}

	Node LoadFile(NxFr::StringView Path)
	{
		return LoadFile(std::string(Path.C()));
	}
}
