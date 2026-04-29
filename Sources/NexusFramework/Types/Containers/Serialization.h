#pragma once

#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/ContainersUtils.h"

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
			Array<T> Data = ContainersUtils::ToArray<T>(Object);
			NxFr::ContainersUtils::Sort<T>(Data);

			Rbs.WriteObject(Data.GetCount());
			for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
			{
				Rbs.WriteObject(Data[Index]);
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
			Array<KeyValuePair<K, T>> Data = NxFr::ContainersUtils::ToArray<K, T>(Object);
			NxFr::ContainersUtils::Sort<NxFr::KeyValuePair<K, T>>(Data);

			Rbs.WriteObject(Data.GetCount());
			for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
			{
				Rbs.WriteObject(Data[Index].Key);
				Rbs.WriteObject(Data[Index].Value);
			}
		}
	};
}

namespace YAML
{
	template<typename T>
	struct convert<NxFr::Array<T>>
	{
		static Node encode(const NxFr::Array<T>& rhs)
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

		static bool decode(const Node& node, NxFr::Array<T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			NxFr::ContainersUtils::Resize<T>(rhs, node.size());
			for (uint64 Index = 0; Index < node.size(); ++Index)
			{
				rhs[Index] = node[Index].as<T>();
			}
			return true;
		}
	};

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Array<T>& rhs)
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

			rhs.Reserve(node.size());
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

			NxFr::Array<T> Data = NxFr::ContainersUtils::ToArray<T>(rhs);
			NxFr::ContainersUtils::Sort<T>(Data);

			for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
			{
				node.push_back(Data[Index]);
			}

			return node;
		}

		static bool decode(const Node& node, NxFr::Set<T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.Reserve(node.size());
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

		NxFr::Array<T> Data = NxFr::ContainersUtils::ToArray<T>(rhs);
		NxFr::ContainersUtils::Sort<T>(Data);

		out << YAML::BeginSeq;
		for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
		{
			out << Data[Index];
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

			NxFr::Array<NxFr::KeyValuePair<K, T>> Data = NxFr::ContainersUtils::ToArray<K, T>(rhs);
			NxFr::ContainersUtils::Sort<NxFr::KeyValuePair<K, T>>(Data);

			for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
			{
				node[Data[Index].Key] = Data[Index].Value;
			}

			return node;
		}

		static bool decode(const Node& node, NxFr::Dictionary<K, T>& rhs)
		{
			if (!node.IsMap())
			{
				return false;
			}

			rhs.Reserve(node.size());
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

		NxFr::Array<NxFr::KeyValuePair<K, T>> Data = NxFr::ContainersUtils::ToArray<K, T>(rhs);
		NxFr::ContainersUtils::Sort<NxFr::KeyValuePair<K, T>>(Data);

		out << YAML::BeginMap;
		for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
		{
			out << YAML::Key << Data[Index].Key;
			out << YAML::Value << Data[Index].Value;
		}
		out << YAML::EndMap;

		return out;
	}
}
