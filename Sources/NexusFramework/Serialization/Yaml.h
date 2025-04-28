#pragma once

#include "NexusFramework/External/YamlCpp.h"

#include "NexusFramework/Core/NexusFrameworkCore.h"
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
		const uint64 SmallSequence = 10;

		NEXUS_FRAMEWORK_API String Serialize(const YAML::Node& Data);
		NEXUS_FRAMEWORK_API String Serialize(const YAML::Emitter& Data);
		NEXUS_FRAMEWORK_API void SerializeFile(const YAML::Node& Data, StringView Path);
		NEXUS_FRAMEWORK_API void SerializeFile(const YAML::Emitter& Data, StringView Path);
		NEXUS_FRAMEWORK_API YAML::Node Deserialize(StringView Data);
		NEXUS_FRAMEWORK_API YAML::Node DeserializeFile(StringView Path);
	}
}

namespace YAML
{
	template<>
	struct convert<NxFr::String>
	{
		NEXUS_FRAMEWORK_API static Node encode(const NxFr::String& rhs);
		NEXUS_FRAMEWORK_API static bool decode(const Node& node, NxFr::String& rhs);
	};
	NEXUS_FRAMEWORK_API YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::String& rhs);

	template<>
	struct convert<NxFr::StringView>
	{
		NEXUS_FRAMEWORK_API static Node encode(const NxFr::StringView& rhs);
		NEXUS_FRAMEWORK_API static bool decode(const Node& node, NxFr::StringView& rhs);
	};
	NEXUS_FRAMEWORK_API YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringView& rhs);

	template<>
	struct convert<NxFr::StringId>
	{
		NEXUS_FRAMEWORK_API static Node encode(const NxFr::StringId& rhs);
		NEXUS_FRAMEWORK_API static bool decode(const Node& node, NxFr::StringId& rhs);
	};
	NEXUS_FRAMEWORK_API YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringId& rhs);

	template<typename T, uint64 N>
	struct convert<NxFr::Array<T, N>>
	{
		static Node encode(const NxFr::Array<T, N>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

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

	template<typename T, uint64 N>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Array<T, N>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::List<T>>
	{
		static Node encode(const NxFr::List<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

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
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::List<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::Set<T>>
	{
		static Node encode(const NxFr::Set<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

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

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Set<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename K, typename T>
	struct convert<NxFr::Dictionary<K, T>>
	{
		static Node encode(const NxFr::Dictionary<K, T>& rhs)
		{
			Node node;
			node[""];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (auto& It : rhs)
			{
				node[It.Key] = It.Value;
			}
			return node;
		}

		static bool decode(const Node& node, NxFr::Dictionary<K, T>& rhs)
		{
			if (!node.IsMap())
			{
				return false;
			}

			rhs.Grow(NxFr::Math::NextPrime(node.size()));
			for (YAML::const_iterator It = node.begin(); It != node.end(); ++It)
			{
				rhs.Append(It->first.as<K>(), It->second.as<T>());
			}
			return true;
		}
	};

	template<typename K, typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Dictionary<K, T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginMap;
		for (auto& It : rhs)
		{
			out << YAML::Key << It.Key;
			out << YAML::Value << It.Value;
		}
		out << YAML::EndMap;

		return out;
	}
}
