#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	NxFr::String Data = R"(name: "NexusProject"
version: 2
value: 3.14
enabled: true

object:
 width: 1280
 height: 720
 fullscreen: false
 title: "Nexus"

array: [3, 5, 7, 11, 13]

list:
 - "Alice"
 - "Bob"
 - "Charlie"

map:
 key: value
 first: second
 id: data

nested:
 created: "2025-04-12"
 tags:
  - engine
  - test
  - yaml
 config:
  debug: true
  max_fps: 144
  gravity: 9.81

empty_list: []
empty_map: {})";

	struct YamlTest
	{
		uint64 Width;
		uint64 Height;
		bool FullScreen;
		NxFr::String Title;
	};

	bool operator==(const YamlTest& A, const YamlTest& B)
	{
		return A.Width == B.Width && A.Height == B.Height && A.FullScreen == B.FullScreen && A.Title == B.Title;
	}

	static YamlTest YamlTestReference = { 1280, 720, false, "Nexus" };
}

namespace YAML
{
	template<>
	struct convert<NxTs::YamlTest>
	{
		static bool decode(const Node& node, NxTs::YamlTest& rhs)
		{
			rhs.Width = node["width"].as<uint64>();
			rhs.Height = node["height"].as<uint64>();
			rhs.FullScreen = node["fullscreen"].as<bool>();
			rhs.Title = node["title"].as<NxFr::String>();
			return true;
		}
	};
}

namespace NxTs
{
	TEST(Serialization, Yaml)
	{
		NxFr::Yaml::Node Yaml = NxFr::Yaml::Load(Data);

		NxFr::String Name = Yaml["name"].as<NxFr::String>();
		ASSERT_EQ(Name, "NexusProject");

		bool Enabled = Yaml["enabled"].as<bool>();
		ASSERT_EQ(Enabled, true);

		uint64 Version = Yaml["version"].as<uint64>();
		ASSERT_EQ(Version, 2);

		float Value = Yaml["value"].as<float>();
		ASSERT_EQ(Value, 3.14f);

		YamlTest Object = Yaml["object"].as<YamlTest>();
		ASSERT_EQ(Object, YamlTestReference);

		NxFr::Yaml::Node Array = Yaml["array"];
		ASSERT_EQ(Array[0].as<uint64>(), 3);
		ASSERT_EQ(Array[2].as<uint64>(), 7);
		ASSERT_EQ(Array[4].as<uint64>(), 13);
		NxFr::Array<uint64, 5> ArrayValue;
		for (uint64 Index = 0; Index < Array.size(); Index++)
		{
			ArrayValue[Index] = Array[Index].as<uint64>();
		}
		ASSERT_EQ(ArrayValue.GetCount(), 5);

		NxFr::Yaml::Node List = Yaml["list"];
		ASSERT_EQ(List[0].as<NxFr::String>(), "Alice");
		ASSERT_EQ(List[1].as<NxFr::String>(), "Bob");
		ASSERT_EQ(List[2].as<NxFr::String>(), "Charlie");
		NxFr::List<NxFr::String> ListValue;
		for (NxFr::Yaml::ConstIterator It = List.begin(); It != List.end(); ++It)
		{
			ListValue.Append(It->as<NxFr::String>());
		}
		ASSERT_EQ(ListValue.GetCount(), 3);

		NxFr::Yaml::Node Map = Yaml["map"];
		ASSERT_EQ(Map["key"].as<NxFr::String>(), "value");
		ASSERT_EQ(Map["first"].as<NxFr::String>(), "second");
		ASSERT_EQ(Map["id"].as<NxFr::String>(), "data");
		NxFr::Dictionary<NxFr::String, NxFr::String> MapValue;
		for (NxFr::Yaml::ConstIterator It = Map.begin(); It != Map.end(); ++It)
		{
			MapValue.Append(It->first.as<NxFr::String>(), It->second.as<NxFr::String>());
		}
		ASSERT_EQ(MapValue.GetCount(), 3);

		NxFr::Yaml::Node Nested = Yaml["nested"];
		ASSERT_EQ(Nested["created"].as<NxFr::String>(), "2025-04-12");
		ASSERT_EQ(Nested["tags"][0].as<NxFr::String>(), "engine");
		ASSERT_EQ(Nested["config"]["gravity"].as<float>(), 9.81f);

		ASSERT_EQ(Yaml["empty_list"].size(), 0);
		ASSERT_EQ(Yaml["empty_map"].size(), 0);
	}
}
