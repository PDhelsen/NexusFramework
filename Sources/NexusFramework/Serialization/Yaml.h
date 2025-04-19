#pragma once

#include "NexusFramework/External/YamlCpp.h"

#include "NexusFramework/Misc/Math.h"

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"

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

	template<typename T, uint64 N>
	struct convert<NxFr::Array<T, N>>
	{
		static bool decode(const Node& node, NxFr::Array<T, N>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			for (uint64 Index = 0; Index < node.size(); ++Index)
			{
				rhs[Index] = node[Index].as<T>();
			}
			return true;
		}
	};

	template<typename T>
	struct convert<NxFr::List<T>>
	{
		static bool decode(const Node& node, NxFr::List<T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.Grow(node.size());
			for (YAML::const_iterator It = node.begin(); It != node.end(); ++It)
			{
				rhs.Append(It->as<T>());
			}
			return true;
		}
	};

	template<typename T>
	struct convert<NxFr::Set<T>>
	{
		static bool decode(const Node& node, NxFr::Set<T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.Grow(NxFr::Math::NextPrime(node.size()));
			for (YAML::const_iterator It = node.begin(); It != node.end(); ++It)
			{
				rhs.Append(It->as<T>());
			}
			return true;
		}
	};

	template<typename K, typename T>
	struct convert<NxFr::Dictionary<K, T>>
	{
		static bool decode(const Node& node, NxFr::Dictionary<K, T>& rhs)
		{
			if (!node.IsMap())
			{
				return false;
			}

			rhs.Grow(NxFr::Math::NextPrime(node.size()));
			for (YAML::const_iterator It = node.begin(); It != node.end(); ++It)
			{
				rhs.Append(It->first.as<T>(), It->second.as<T>());
			}
			return true;
		}
	};
}
