#pragma once

#include "NexusFramework/External/YamlCpp.h"

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Math/Math.h"

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"
#include "NexusFramework/Math/Matrix.h"
#include "NexusFramework/Math/Shape.h"

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

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::String& rhs)
	{
		out << rhs.C();
		return out;
	}

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

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringView& rhs)
	{
		out << rhs.C();
		return out;
	}

	template<>
	struct convert<NxFr::StringId>
	{
		static Node encode(const NxFr::StringId& rhs)
		{
			return Node(rhs.C());
		}

		static bool decode(const Node& node, NxFr::StringId& rhs)
		{
			if (!node.IsScalar())
			{
				return false;
			}

			rhs = NxFr::StringId(node.as<NxFr::StringView>());
			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringId& rhs)
	{
		out << rhs.C();
		return out;
	}

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

	template<uint8 D, typename T>
	struct convert<NxFr::Vector<D, T>>
	{
		static Node encode(const NxFr::Vector<D, T>& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			for (uint8 Index = 0; Index < D; ++Index)
			{
				node.push_back(rhs[Index]);
			}

			return node;
		}

		static bool decode(const Node& node, NxFr::Vector<D, T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			for (uint8 Index = 0; Index < node.size(); ++Index)
			{
				rhs[Index] = node[Index].as<T>();
			}

			return true;
		}
	};

	template<uint8 D, typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Vector<D, T>& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for (uint8 Index = 0; Index < D; ++Index)
		{
			out << rhs[Index];
		}
		out << YAML::EndSeq;

		return out;
	}

	template<>
	struct convert<NxFr::Euler>
	{
		static Node encode(const NxFr::Euler& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, NxFr::Euler& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Euler& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;

		out << rhs.x;
		out << rhs.y;
		out << rhs.z;

		out << YAML::EndSeq;

		return out;
	}

	template<>
	struct convert<NxFr::AxisAngle>
	{
		static Node encode(const NxFr::AxisAngle& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			node.push_back(rhs.Angle);
			node.push_back(rhs.Axis.x);
			node.push_back(rhs.Axis.y);
			node.push_back(rhs.Axis.z);

			return node;
		}

		static bool decode(const Node& node, NxFr::AxisAngle& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.Angle = node[0].as<float>();
			rhs.Axis.x = node[1].as<float>();
			rhs.Axis.y = node[2].as<float>();
			rhs.Axis.z = node[3].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::AxisAngle& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;

		out << rhs.Angle;
		out << rhs.Axis.x;
		out << rhs.Axis.y;
		out << rhs.Axis.z;

		out << YAML::EndSeq;

		return out;
	}

	template<>
	struct convert<NxFr::Quaternion>
	{
		static Node encode(const NxFr::Quaternion& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			node.push_back(rhs.w);
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, NxFr::Quaternion& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.w = node[0].as<float>();
			rhs.z = node[1].as<float>();
			rhs.y = node[2].as<float>();
			rhs.z = node[3].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Quaternion& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;

		out << rhs.w;
		out << rhs.x;
		out << rhs.y;
		out << rhs.z;

		out << YAML::EndSeq;

		return out;
	}

	template<uint8 R, uint8 C, typename T>
	struct convert<NxFr::Matrix<R, C, T>>
	{
		static Node encode(const NxFr::Matrix<R, C, T>& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			for (uint8 Index = 0; Index < NxFr::Matrix<R, C, T>::Count; ++Index)
			{
				node.push_back(rhs[Index]);
			}

			return node;
		}

		static bool decode(const Node& node, NxFr::Matrix<R, C, T>& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			for (uint8 Index = 0; Index < node.size(); ++Index)
			{
				rhs[Index] = node[Index].as<T>();
			}

			return true;
		}
	};

	template<uint8 R, uint8 C, typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Matrix<R, C, T>& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for (uint8 Index = 0; Index < NxFr::Matrix<R, C, T>::Count; ++Index)
		{
			out << rhs[Index];
		}
		out << YAML::EndSeq;

		return out;
	}

	template<>
	struct convert<NxFr::Ray>
	{
		static Node encode(const NxFr::Ray& rhs)
		{
			Node node;

			node["origin"] = rhs.Origin;
			node["direction"] = rhs.Direction;

			return node;
		}

		static bool decode(const Node& node, NxFr::Ray& rhs)
		{
			rhs.Origin = node["origin"].as<NxFr::Vector3f>();
			rhs.Direction = node["direction"].as<NxFr::Vector3f>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Ray& rhs)
	{
		out << YAML::Key << "origin" << YAML::Value << rhs.Origin;
		out << YAML::Key << "direction" << YAML::Value << rhs.Direction;

		return out;
	}

	template<>
	struct convert<NxFr::Plane>
	{
		static Node encode(const NxFr::Plane& rhs)
		{
			Node node;

			node["normal"] = rhs.Normal;
			node["distance"] = rhs.Distance;

			return node;
		}

		static bool decode(const Node& node, NxFr::Plane& rhs)
		{
			rhs.Normal = node["normal"].as<NxFr::Vector3f>();
			rhs.Distance = node["distance"].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Plane& rhs)
	{
		out << YAML::Key << "normal" << YAML::Value << rhs.Normal;
		out << YAML::Key << "distance" << YAML::Value << rhs.Distance;

		return out;
	}

	template<>
	struct convert<NxFr::Triangle>
	{
		static Node encode(const NxFr::Triangle& rhs)
		{
			Node node;

			node["a"] = rhs.A;
			node["b"] = rhs.B;
			node["c"] = rhs.C;

			return node;
		}

		static bool decode(const Node& node, NxFr::Triangle& rhs)
		{
			rhs.A = node["a"].as<NxFr::Vector3f>();
			rhs.B = node["b"].as<NxFr::Vector3f>();
			rhs.C = node["c"].as<NxFr::Vector3f>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Triangle& rhs)
	{
		out << YAML::Key << "a" << YAML::Value << rhs.A;
		out << YAML::Key << "b" << YAML::Value << rhs.B;
		out << YAML::Key << "c" << YAML::Value << rhs.C;

		return out;
	}

	template<>
	struct convert<NxFr::Rectangle>
	{
		static Node encode(const NxFr::Rectangle& rhs)
		{
			Node node;

			node["center"] = rhs.Center;
			node["extents"] = rhs.Extents;

			return node;
		}

		static bool decode(const Node& node, NxFr::Rectangle& rhs)
		{
			rhs.Center = node["center"].as<NxFr::Vector2f>();
			rhs.Extents = node["extents"].as<NxFr::Vector2f>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Rectangle& rhs)
	{
		out << YAML::Key << "center" << YAML::Value << rhs.Center;
		out << YAML::Key << "extents" << YAML::Value << rhs.Extents;

		return out;
	}

	template<>
	struct convert<NxFr::Circle>
	{
		static Node encode(const NxFr::Circle& rhs)
		{
			Node node;

			node["center"] = rhs.Center;
			node["radius"] = rhs.Radius;

			return node;
		}

		static bool decode(const Node& node, NxFr::Circle& rhs)
		{
			rhs.Center = node["center"].as<NxFr::Vector2f>();
			rhs.Radius = node["radius"].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Circle& rhs)
	{
		out << YAML::Key << "center" << YAML::Value << rhs.Center;
		out << YAML::Key << "radius" << YAML::Value << rhs.Radius;

		return out;
	}

	template<>
	struct convert<NxFr::Box>
	{
		static Node encode(const NxFr::Box& rhs)
		{
			Node node;

			node["center"] = rhs.Center;
			node["extents"] = rhs.Extents;

			return node;
		}

		static bool decode(const Node& node, NxFr::Box& rhs)
		{
			rhs.Center = node["center"].as<NxFr::Vector3f>();
			rhs.Extents = node["extents"].as<NxFr::Vector3f>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Box& rhs)
	{
		out << YAML::Key << "center" << YAML::Value << rhs.Center;
		out << YAML::Key << "extents" << YAML::Value << rhs.Extents;

		return out;
	}

	template<>
	struct convert<NxFr::Cuboid>
	{
		static Node encode(const NxFr::Cuboid& rhs)
		{
			Node node;

			node["center"] = rhs.Center;
			node["orientation"] = rhs.Orientation;
			node["extents"] = rhs.Extents;

			return node;
		}

		static bool decode(const Node& node, NxFr::Cuboid& rhs)
		{
			rhs.Center = node["center"].as<NxFr::Vector3f>();
			rhs.Orientation = node["orientation"].as<NxFr::Quaternion>();
			rhs.Extents = node["extents"].as<NxFr::Vector3f>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Cuboid& rhs)
	{
		out << YAML::Key << "center" << YAML::Value << rhs.Center;
		out << YAML::Key << "orientation" << YAML::Value << rhs.Orientation;
		out << YAML::Key << "extents" << YAML::Value << rhs.Extents;

		return out;
	}

	template<>
	struct convert<NxFr::Sphere>
	{
		static Node encode(const NxFr::Sphere& rhs)
		{
			Node node;

			node["center"] = rhs.Center;
			node["radius"] = rhs.Radius;

			return node;
		}

		static bool decode(const Node& node, NxFr::Sphere& rhs)
		{
			rhs.Center = node["center"].as<NxFr::Vector3f>();
			rhs.Radius = node["radius"].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Sphere& rhs)
	{
		out << YAML::Key << "center" << YAML::Value << rhs.Center;
		out << YAML::Key << "radius" << YAML::Value << rhs.Radius;

		return out;
	}
}
