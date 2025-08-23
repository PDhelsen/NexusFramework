#pragma once

#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"

#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	template<typename T>
	struct RBSConverter<Array<T>>
	{
		static Array<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Array<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result[Index] = Rbs.ReadObject<T>();
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Array<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (uint64 Index = 0; Index < Object.GetCount(); ++Index)
			{
				Rbs.WriteObject(Object[Index]);
			}
		}
	};

	template<typename T>
	struct RBSConverter<List<T>>
	{
		static List<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			List<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.Append(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const List<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (uint64 Index = 0; Index < Object.GetCount(); ++Index)
			{
				Rbs.WriteObject(Object[Index]);
			}
		}
	};

	template<typename T>
	struct RBSConverter<Set<T>>
	{
		static Set<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Set<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.Append(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Set<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename K, typename T>
	struct RBSConverter<Dictionary<K, T>>
	{
		static Dictionary<K, T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Dictionary<K, T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				K Key = Rbs.ReadObject<K>();
				T Value = Rbs.ReadObject<T>();
				Result.Append(Move(Key), Move(Value));
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Dictionary<K, T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (auto& It : Object)
			{
				Rbs.WriteObject(It.Key);
				Rbs.WriteObject(It.Value);
			}
		}
	};
}

namespace YAML
{
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

			for (uint64 Index = 0; Index < rhs.GetCount(); ++Index)
			{
				node.push_back(rhs[Index]);
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
		for (uint64 Index = 0; Index < rhs.GetCount(); ++Index)
		{
			out << rhs[Index];
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

			for (uint64 Index = 0; Index < rhs.GetCount(); ++Index)
			{
				node.push_back(rhs[Index]);
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
			for (uint64 Index = 0; Index < node.size(); ++Index)
			{
				rhs.Append(node[Index].as<T>());
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
		for (uint64 Index = 0; Index < rhs.GetCount(); ++Index)
		{
			out << rhs[Index];
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
