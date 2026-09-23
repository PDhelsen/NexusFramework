#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Yaml.h"

#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	namespace Yaml
	{
		String Serialize(const Node& Data)
		{
			Allocator::Scope _ = nullptr;
			return YAML::Dump(Data.Data).c_str();
		}

		void SerializeAndSave(const Node& Data, StringView Path)
		{
			TextStream Stream(Path);
			Stream.Open(File::Mode::Write);
			Stream.WriteAll(Serialize(Data));
			Stream.Close();
		}

		Node Deserialize(StringView Data)
		{
			Allocator::Scope _ = nullptr;
			return YAML::Load(Data.C());
		}

		Node LoadAndDeserialize(StringView Path)
		{
			Allocator::Scope _ = nullptr;
			return YAML::LoadFile(Path.C());
		}
	}
}
