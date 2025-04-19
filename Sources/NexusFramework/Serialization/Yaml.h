#pragma once

#include "NexusFramework/External/YamlCpp.h"

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"

namespace NxFr
{
	namespace Yaml
	{
		using Node = YAML::Node;
		using Iterator = YAML::iterator;
		using ConstIterator = YAML::const_iterator;

		NEXUS_FRAMEWORK_API Node Load(NxFr::StringView Data);
		NEXUS_FRAMEWORK_API Node LoadFile(NxFr::StringView Path);
	}
}

namespace YAML
{
	template<>
	struct convert<NxFr::String>
	{
		static Node encode(const NxFr::String& rhs)
		{
			return Node(rhs.C());
		}

		static bool decode(const Node& node, NxFr::String& rhs)
		{
			rhs = node.as<std::string>().c_str();
			return true;
		}
	};

	template<>
	struct convert<NxFr::StringView>
	{
		static Node encode(const NxFr::StringView& rhs)
		{
			return Node(rhs.C());
		}

		static bool decode(const Node& node, NxFr::StringView& rhs)
		{
			rhs = node.as<std::string>().c_str();
			return true;
		}
	};

	template<>
	struct convert<NxFr::StringId>
	{
		static Node encode(const NxFr::StringId& rhs)
		{
			return Node(rhs.GetId());
		}

		static bool decode(const Node& node, NxFr::StringId& rhs)
		{
			if (!node.IsScalar())
			{
				return false;
			}

			rhs = node.as<NxFr::GUID>();
			return true;
		}
	};
}
