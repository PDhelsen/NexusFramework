#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	NxFr::String Data = R"(name: "NexusProject"
version: 2
value: 3.14
enabled: true

window:
 width: 1280
 height: 720
 fullscreen: false
 title: "Nexus Engine Test"

authors:
 -"Alice"
 -"Bob"
 -"Charlie"

metadata:
 created: "2025-04-12"
 tags:
  -engine
  -test
  -yaml
 config:
  debug: true
  max_fps: 144
  gravity: 9.81

empty_list: []
empty_map: {})";

	TEST(Serialization, Yaml)
	{
		YAML::Node Yaml = YAML::Load(Data);

		NxFr::String Name = Yaml["name"].as<NxFr::String>();
		ASSERT_EQ(Name, "NexusProject");

		bool Enabled = Yaml["enabled"].as<bool>();
		ASSERT_EQ(Enabled, true);

		uint64 Version = Yaml["version"].as<uint64>();
		ASSERT_EQ(Version, 2);

		float Value = Yaml["value"].as<float>();
		ASSERT_EQ(Value, 3.14f);
	}
}
