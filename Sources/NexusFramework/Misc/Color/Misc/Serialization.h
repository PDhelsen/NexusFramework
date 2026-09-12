#pragma once

#include "NexusFramework/Misc/Color/Color.h"

#include "NexusFramework/Serialization/Yaml.h"
#include "NexusFramework/Serialization/Rbs.h"

namespace NxFr
{
	namespace RBSUtility
	{
		template<>
		struct Converter<Color>
		{
			static Color Decode(const RBS& Rbs)
			{
				Color Result;
				Result.r = Rbs.ReadObject<Color::Type>();
				Result.g = Rbs.ReadObject<Color::Type>();
				Result.b = Rbs.ReadObject<Color::Type>();
				Result.a = Rbs.ReadObject<Color::Type>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Color& Object)
			{
				Rbs.WriteObject(Object.r);
				Rbs.WriteObject(Object.g);
				Rbs.WriteObject(Object.b);
				Rbs.WriteObject(Object.a);
			}
		};
	}
}

namespace YAML
{
	template<>
	struct convert<NxFr::Color>
	{
		static Node encode(const NxFr::Color& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);

			return node;
		}

		static bool decode(const Node& node, NxFr::Color& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.r = node[0].as<NxFr::Color::Type>();
			rhs.g = node[1].as<NxFr::Color::Type>();
			rhs.b = node[2].as<NxFr::Color::Type>();
			rhs.a = node[3].as<NxFr::Color::Type>();

			return true;
		}
	};
}
