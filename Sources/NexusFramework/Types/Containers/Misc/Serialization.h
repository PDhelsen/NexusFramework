#pragma once

#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"

#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dequeue.h"
#include "NexusFramework/Types/Containers/Queue.h"
#include "NexusFramework/Types/Containers/Stack.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/LinkedList.h"
#include "NexusFramework/Types/Containers/Tree.h"
#include "NexusFramework/Types/Containers/Graph.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Types/Containers/Tuple.h"
#include "NexusFramework/Types/Containers/Collection.h"
#include "NexusFramework/Types/Containers/ContainersUtils.h"

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
			for (const auto& It : Object)
			{
				Rbs.WriteObject(It);
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
			for (const auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename T>
	struct RBSConverter<Dequeue<T>>
	{
		static Dequeue<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Dequeue<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.AppendBack(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Dequeue<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (const auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename T>
	struct RBSConverter<Queue<T>>
	{
		static Queue<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Queue<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.Append(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Queue<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (const auto& It : Object)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename T>
	struct RBSConverter<Stack<T>>
	{
		static Queue<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			Stack<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.Append(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const Stack<T>& Object)
		{
			Array<T> Data = ContainersUtils::ToArray<T>(Object);
			ContainersUtils::Reverse<T>(Data);

			Rbs.WriteObject(Data.GetCount());
			for (const auto& It : Data)
			{
				Rbs.WriteObject(It);
			}
		}
	};

	template<typename T>
	struct RBSConverter<LinkedList<T>>
	{
		static LinkedList<T> Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			LinkedList<T> Result(Size);
			for (uint64 Index = 0; Index < Size; ++Index)
			{
				Result.AppendBack(Rbs.ReadObject<T>());
			}
			return Result;
		}

		static void Encode(RBS& Rbs, const LinkedList<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (const auto& It : Object)
			{
				Rbs.WriteObject(It);
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
			ContainersUtils::Sort<T>(Data);

			Rbs.WriteObject(Data.GetCount());
			for (const auto& It : Data)
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
			Array<KeyValuePair<K, T>> Data = ContainersUtils::ToArray<K, T>(Object);
			ContainersUtils::Sort<KeyValuePair<K, T>>(Data);

			Rbs.WriteObject(Data.GetCount());
			for (const auto& It : Data)
			{
				Rbs.WriteObject(It.Key);
				Rbs.WriteObject(It.Value);
			}
		}
	};

	template<>
	struct RBSConverter<BufferView>
	{
		static BufferView Decode(RBS& Rbs)
		{
			uint64 Size = Rbs.ReadObject<uint64>();
			const void* Ptr = Rbs.ReadByte(Size);
			return BufferView(Ptr, Size);
		}

		static void Encode(RBS& Rbs, const BufferView& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			Rbs.WriteByte(Object.GetPtr(), Object.GetCount());
		}
	};

	template<typename T1, typename T2>
	struct RBSConverter<Tuple<T1, T2>>
	{
		static Tuple<T1, T2> Decode(RBS& Rbs)
		{
			return Tuple<T1, T2>(Rbs.ReadObject<T1>(), Rbs.ReadObject<T2>());
		}

		static void Encode(RBS& Rbs, const Tuple<T1, T2>& Object)
		{
			Rbs.WriteObject(Object.GetFirst());
			Rbs.WriteObject(Object.GetSecond());
		}
	};

	template<typename T>
	struct RBSConverter<Collection<T>>
	{
		static Array<T> Decode(RBS& Rbs)
		{
			NEXUS_ASSERT(false, Default, "Unsupported Decode with Collection");
		}

		static void Encode(RBS& Rbs, const Collection<T>& Object)
		{
			Rbs.WriteObject(Object.GetCount());
			for (const auto& It : Object)
			{
				Rbs.WriteObject(It);
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

			for (const auto& It : rhs)
			{
				node.push_back(It);
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
		for (const auto& It : rhs)
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

			for (const auto& It : rhs)
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
		for (const auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::Dequeue<T>>
	{
		static Node encode(const NxFr::Dequeue<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (const auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

		static bool decode(const Node& node, NxFr::Dequeue<T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.Reserve(node.size());
			for (uint64 Index = 0; Index < node.size(); ++Index)
			{
				rhs.AppendBack(node[Index].as<T>());
			}
			return true;
		}
	};

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Dequeue<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (const auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::Queue<T>>
	{
		static Node encode(const NxFr::Queue<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (const auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

		static bool decode(const Node& node, NxFr::Queue<T>& rhs)
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
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Queue<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (const auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::Stack<T>>
	{
		static Node encode(const NxFr::Stack<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			NxFr::Array<T> Data = NxFr::ContainersUtils::ToArray<T>(rhs);
			NxFr::ContainersUtils::Reverse<T>(Data);

			for (const auto& It : Data)
			{
				node.push_back(It);
			}
			return node;
		}

		static bool decode(const Node& node, NxFr::Stack<T>& rhs)
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
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Stack<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		NxFr::Array<T> Data = NxFr::ContainersUtils::ToArray<T>(rhs);
		NxFr::ContainersUtils::Reverse<T>(Data);

		out << YAML::BeginSeq;
		for (const auto& It : Data)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::LinkedList<T>>
	{
		static Node encode(const NxFr::LinkedList<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (const auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

		static bool decode(const Node& node, NxFr::LinkedList<T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.Reserve(node.size());
			for (uint64 Index = 0; Index < node.size(); ++Index)
			{
				rhs.AppendBack(node[Index].as<T>());
			}
			return true;
		}
	};

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::LinkedList<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (const auto& It : rhs)
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

			NxFr::Array<T> Data = NxFr::ContainersUtils::ToArray<T>(rhs);
			NxFr::ContainersUtils::Sort<T>(Data);

			for (const auto& It : Data)
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
		for (const auto& It : Data)
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

	template<typename T1, typename T2>
	struct convert<NxFr::Tuple<T1, T2>>
	{
		static Node encode(const NxFr::Tuple<T1, T2>& rhs)
		{
			Node node;

			node.SetStyle(YAML::EmitterStyle::Flow);
			node[0] = rhs.GetFirst();
			node[1] = rhs.GetSecond();

			return node;
		}

		static bool decode(const Node& node, NxFr::Tuple<T1, T2>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs[0].SetFirst(node[0].as<T1>());
			rhs[1].SetSecond(node[1].as<T2>());

			return true;
		}
	};

	template<typename T1, typename T2>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Tuple<T1, T2>& rhs)
	{
		out << YAML::Flow;

		out << YAML::BeginSeq;
		out << rhs.GetFirst();
		out << rhs.GetSecond();
		out << YAML::EndSeq;

		return out;
	}

	template<typename T>
	struct convert<NxFr::Collection<T>>
	{
		static Node encode(const NxFr::Collection<T>& rhs)
		{
			Node node;
			node[0];

			if (rhs.GetCount() < NxFr::Yaml::SmallSequence)
			{
				node.SetStyle(YAML::EmitterStyle::Flow);
			}

			for (const auto& It : rhs)
			{
				node.push_back(It);
			}
			return node;
		}

		static bool decode(const Node& node, NxFr::Collection<T>& rhs)
		{
			NEXUS_ASSERT(false, Default, "Unsupported Decode with Collection");
			return false;
		}
	};

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Collection<T>& rhs)
	{
		if (rhs.GetCount() <= NxFr::Yaml::SmallSequence)
		{
			out << YAML::Flow;
		}

		out << YAML::BeginSeq;
		for (const auto& It : rhs)
		{
			out << It;
		}
		out << YAML::EndSeq;

		return out;
	}
}
