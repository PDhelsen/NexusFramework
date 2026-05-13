#pragma once

#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"
#include "NexusFramework/Math/Matrix.h"
#include "NexusFramework/Math/Shape.h"

#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	namespace RBSUtility
	{
		template<uint8 D, typename T>
		struct Converter<Vector<D, T>>
		{
			static Vector<D, T> Decode(const RBS& Rbs)
			{
				Vector<D, T> Result;
				for (uint8 I = 0; I < D; ++I)
				{
					Result[I] = Rbs.ReadObject<T>();
				}
				return Result;
			}

			static void Encode(RBS& Rbs, const Vector<D, T>& Object)
			{
				for (uint8 I = 0; I < D; ++I)
				{
					Rbs.WriteObject(Object[I]);
				}
			}
		};

		template<>
		struct Converter<Euler>
		{
			static Euler Decode(const RBS& Rbs)
			{
				Euler Result;
				Result.x = Rbs.ReadObject<float>();
				Result.y = Rbs.ReadObject<float>();
				Result.z = Rbs.ReadObject<float>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Euler& Object)
			{
				Rbs.WriteObject(Object.x);
				Rbs.WriteObject(Object.y);
				Rbs.WriteObject(Object.z);
			}
		};

		template<>
		struct Converter<AxisAngle>
		{
			static AxisAngle Decode(const RBS& Rbs)
			{
				AxisAngle Result;
				Result.Angle = Rbs.ReadObject<float>();
				Result.Axis.x = Rbs.ReadObject<float>();
				Result.Axis.y = Rbs.ReadObject<float>();
				Result.Axis.z = Rbs.ReadObject<float>();
				return Result;
			}

			static void Encode(RBS& Rbs, const AxisAngle& Object)
			{
				Rbs.WriteObject(Object.Angle);
				Rbs.WriteObject(Object.Axis.x);
				Rbs.WriteObject(Object.Axis.y);
				Rbs.WriteObject(Object.Axis.z);
			}
		};

		template<>
		struct Converter<Quaternion>
		{
			static Quaternion Decode(const RBS& Rbs)
			{
				Quaternion Result;
				Result.w = Rbs.ReadObject<float>();
				Result.x = Rbs.ReadObject<float>();
				Result.y = Rbs.ReadObject<float>();
				Result.z = Rbs.ReadObject<float>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Quaternion& Object)
			{
				Rbs.WriteObject(Object.w);
				Rbs.WriteObject(Object.x);
				Rbs.WriteObject(Object.y);
				Rbs.WriteObject(Object.z);
			}
		};

		template<uint8 R, uint8 C, typename T>
		struct Converter<Matrix<R, C, T>>
		{
			static Matrix<R, C, T> Decode(const RBS& Rbs)
			{
				Matrix<R, C, T> Result;
				for (uint8 I = 0; I < Matrix<R, C, T>::Count; ++I)
				{
					Result[I] = Rbs.ReadObject<T>();
				}
				return Result;
			}

			static void Encode(RBS& Rbs, const Matrix<R, C, T>& Object)
			{
				for (uint8 I = 0; I < Matrix<R, C, T>::Count; ++I)
				{
					Rbs.WriteObject(Object[I]);
				}
			}
		};

		template<>
		struct Converter<Ray>
		{
			static Ray Decode(const RBS& Rbs)
			{
				Ray Result;
				Result.Origin = Rbs.ReadObject<Vector3f>();
				Result.Direction = Rbs.ReadObject<Vector3f>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Ray& Object)
			{
				Rbs.WriteObject(Object.Origin);
				Rbs.WriteObject(Object.Direction);
			}
		};

		template<>
		struct Converter<Plane>
		{
			static Plane Decode(const RBS& Rbs)
			{
				Plane Result;
				Result.Normal = Rbs.ReadObject<Vector3f>();
				Result.Distance = Rbs.ReadObject<float>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Plane& Object)
			{
				Rbs.WriteObject(Object.Normal);
				Rbs.WriteObject(Object.Distance);
			}
		};

		template<>
		struct Converter<Triangle>
		{
			static Triangle Decode(const RBS& Rbs)
			{
				Triangle Result;
				Result.A = Rbs.ReadObject<Vector3f>();
				Result.B = Rbs.ReadObject<Vector3f>();
				Result.C = Rbs.ReadObject<Vector3f>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Triangle& Object)
			{
				Rbs.WriteObject(Object.A);
				Rbs.WriteObject(Object.B);
				Rbs.WriteObject(Object.C);
			}
		};

		template<>
		struct Converter<Rectangle>
		{
			static Rectangle Decode(const RBS& Rbs)
			{
				Rectangle Result;
				Result.Center = Rbs.ReadObject<Vector2f>();
				Result.Extents = Rbs.ReadObject<Vector2f>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Rectangle& Object)
			{
				Rbs.WriteObject(Object.Center);
				Rbs.WriteObject(Object.Extents);
			}
		};

		template<>
		struct Converter<Circle>
		{
			static Circle Decode(const RBS& Rbs)
			{
				Circle Result;
				Result.Center = Rbs.ReadObject<Vector2f>();
				Result.Radius = Rbs.ReadObject<float>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Circle& Object)
			{
				Rbs.WriteObject(Object.Center);
				Rbs.WriteObject(Object.Radius);
			}
		};

		template<>
		struct Converter<Box>
		{
			static Box Decode(const RBS& Rbs)
			{
				Box Result;
				Result.Center = Rbs.ReadObject<Vector3f>();
				Result.Extents = Rbs.ReadObject<Vector3f>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Box& Object)
			{
				Rbs.WriteObject(Object.Center);
				Rbs.WriteObject(Object.Extents);
			}
		};

		template<>
		struct Converter<Cube>
		{
			static Cube Decode(const RBS& Rbs)
			{
				Cube Result;
				Result.Center = Rbs.ReadObject<Vector3f>();
				Result.Orientation = Rbs.ReadObject<Quaternion>();
				Result.Extents = Rbs.ReadObject<Vector3f>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Cube& Object)
			{
				Rbs.WriteObject(Object.Center);
				Rbs.WriteObject(Object.Orientation);
				Rbs.WriteObject(Object.Extents);
			}
		};

		template<>
		struct Converter<Sphere>
		{
			static Sphere Decode(const RBS& Rbs)
			{
				Sphere Result;
				Result.Center = Rbs.ReadObject<Vector3f>();
				Result.Radius = Rbs.ReadObject<float>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Sphere& Object)
			{
				Rbs.WriteObject(Object.Center);
				Rbs.WriteObject(Object.Radius);
			}
		};
	}
}

namespace YAML
{
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
	struct convert<NxFr::Cube>
	{
		static Node encode(const NxFr::Cube& rhs)
		{
			Node node;

			node["center"] = rhs.Center;
			node["orientation"] = rhs.Orientation;
			node["extents"] = rhs.Extents;

			return node;
		}

		static bool decode(const Node& node, NxFr::Cube& rhs)
		{
			rhs.Center = node["center"].as<NxFr::Vector3f>();
			rhs.Orientation = node["orientation"].as<NxFr::Quaternion>();
			rhs.Extents = node["extents"].as<NxFr::Vector3f>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Cube& rhs)
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
