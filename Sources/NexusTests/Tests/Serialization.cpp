#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	NxFr::String Data = R"(name: NexusProject
enabled: true
version: 2
value: 3.14
object:
  width: 1280
  height: 720
  fullscreen: false
  title: Nexus
array: [3, 5, 7, 11, 13]
list: [Alice, Bob, Charlie]
map: {id: data, key: value, first: second}
nested:
  created: 2025-04-12
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

	struct SerializationTest
	{
		uint64 Width = 0;
		uint64 Height = 0;
		bool FullScreen = false;
		NxFr::String Title = "";
	};

	bool operator==(const SerializationTest& A, const SerializationTest& B)
	{
		return A.Width == B.Width && A.Height == B.Height && A.FullScreen == B.FullScreen && A.Title == B.Title;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const SerializationTest& rhs)
	{
		out << YAML::Key << "width" << YAML::Value << rhs.Width;
		out << YAML::Key << "height" << YAML::Value << rhs.Height;
		out << YAML::Key << "fullscreen" << YAML::Value << rhs.FullScreen;
		out << YAML::Key << "title" << YAML::Value << rhs.Title.C();
		return out;
	}

	static SerializationTest YamlTestReference = { 1280, 720, false, "Nexus" };
}

namespace YAML
{
	template<>
	struct convert<NxTs::SerializationTest>
	{
		static Node encode(const NxTs::SerializationTest& rhs)
		{
			Node node;
			node["width"] = rhs.Width;
			node["height"] = rhs.Height;
			node["fullscreen"] = rhs.FullScreen;
			node["title"] = rhs.Title;
			return node;
		}

		static bool decode(const Node& node, NxTs::SerializationTest& rhs)
		{
			rhs.Width = node["width"].as<uint64>();
			rhs.Height = node["height"].as<uint64>();
			rhs.FullScreen = node["fullscreen"].as<bool>();
			rhs.Title = node["title"].as<NxFr::String>();
			return true;
		}
	};
}

namespace NxFr
{
	template<>
	struct RBSConverter<NxTs::SerializationTest>
	{
		static NxTs::SerializationTest Decode(RBS& Rbs)
		{
			NxTs::SerializationTest Test;
			Test.Width = Rbs.ReadObject<uint64>();
			Test.Height = Rbs.ReadObject<uint64>();
			Test.FullScreen = Rbs.ReadObject<bool>();
			Test.Title = Rbs.ReadObject<String>();
			return Test;
		}

		static void Encode(RBS& Rbs, const NxTs::SerializationTest& Object)
		{
			Rbs.WriteObject(Object.Width);
			Rbs.WriteObject(Object.Height);
			Rbs.WriteObject(Object.FullScreen);
			Rbs.WriteObject(Object.Title);
		}
	};
}

namespace NxTs
{
	TEST(Serialization, Yaml)
	{
		YAML::Node Deserialize = NxFr::Yaml::Deserialize(Data);

		NxFr::String Name = Deserialize["name"].as<NxFr::String>();
		ASSERT_EQ(Name, "NexusProject");
		bool Enabled = Deserialize["enabled"].as<bool>();
		ASSERT_EQ(Enabled, true);
		uint64 Version = Deserialize["version"].as<uint64>();
		ASSERT_EQ(Version, 2);
		float Value = Deserialize["value"].as<float>();
		ASSERT_EQ(Value, 3.14f);
		SerializationTest Object = Deserialize["object"].as<SerializationTest>();
		ASSERT_EQ(Object, YamlTestReference);

		YAML::Node Array = Deserialize["array"];
		ASSERT_EQ(Array[0].as<uint64>(), 3);
		ASSERT_EQ(Array[2].as<uint64>(), 7);
		ASSERT_EQ(Array[4].as<uint64>(), 13);
		NxFr::Array<uint64, 5> ArrayValue;
		for (uint64 Index = 0; Index < Array.size(); Index++)
		{
			ArrayValue[Index] = Array[Index].as<uint64>();
		}
		ASSERT_EQ(ArrayValue.GetCount(), 5);
		NxFr::Array<uint64, 5> ArrayDecoded = Array.as<NxFr::Array<uint64, 5>>();
		ASSERT_EQ(ArrayDecoded.GetCount(), 5);

		YAML::Node List = Deserialize["list"];
		ASSERT_EQ(List[0].as<NxFr::String>(), "Alice");
		ASSERT_EQ(List[1].as<NxFr::String>(), "Bob");
		ASSERT_EQ(List[2].as<NxFr::String>(), "Charlie");
		NxFr::List<NxFr::String> ListValue;
		for (YAML::const_iterator It = List.begin(); It != List.end(); ++It)
		{
			ListValue.Append(It->as<NxFr::String>());
		}
		ASSERT_EQ(ListValue.GetCount(), 3);
		NxFr::List<NxFr::String> ListDecoded = List.as<NxFr::List<NxFr::String>>();
		ASSERT_EQ(ListDecoded.GetCount(), 3);

		YAML::Node Map = Deserialize["map"];
		ASSERT_EQ(Map["key"].as<NxFr::String>(), "value");
		ASSERT_EQ(Map["first"].as<NxFr::String>(), "second");
		ASSERT_EQ(Map["id"].as<NxFr::String>(), "data");
		NxFr::Dictionary<NxFr::String, NxFr::String> MapValue;
		for (YAML::const_iterator It = Map.begin(); It != Map.end(); ++It)
		{
			MapValue.Append(It->first.as<NxFr::String>(), It->second.as<NxFr::String>());
		}
		ASSERT_EQ(MapValue.GetCount(), 3);
		NxFr::Dictionary<NxFr::String, NxFr::String> MapDecoded = Map.as<NxFr::Dictionary<NxFr::String, NxFr::String>>();
		ASSERT_EQ(MapDecoded.GetCount(), 3);

		YAML::Node Nested = Deserialize["nested"];
		ASSERT_EQ(Nested["created"].as<NxFr::String>(), "2025-04-12");
		ASSERT_EQ(Nested["tags"][0].as<NxFr::String>(), "engine");
		ASSERT_EQ(Nested["config"]["gravity"].as<float>(), 9.81f);

		ASSERT_EQ(Deserialize["empty_list"].size(), 0);
		ASSERT_EQ(Deserialize["empty_map"].size(), 0);


		YAML::Node Serialize;

		Serialize["name"] = Name;
		Serialize["enabled"] = Enabled;
		Serialize["version"] = Version;
		Serialize["value"] = Value;
		Serialize["object"] = Object;
		Serialize["array"] = ArrayValue;
		Serialize["list"] = ListValue;
		Serialize["map"] = MapValue;
		Serialize["nested"] = Nested;
		Serialize["empty_list"] = NxFr::List<uint64>();
		Serialize["empty_map"] = NxFr::Dictionary<uint64, uint64>();

		NxFr::String Output = NxFr::Yaml::Serialize(Serialize);
		ASSERT_EQ(Output, Data);


		YAML::Emitter Emitter;

		Emitter << YAML::Key << "name" << YAML::Value << Name;
		Emitter << YAML::Key << "enabled" << YAML::Value << Enabled;
		Emitter << YAML::Key << "version" << YAML::Value << Version;
		Emitter << YAML::Key << "value" << YAML::Value << Value;
		Emitter << YAML::Key << "object" << YAML::Value << Object;
		Emitter << YAML::Key << "array" << YAML::Value << ArrayValue;
		Emitter << YAML::Key << "list" << YAML::Value << ListValue;
		Emitter << YAML::Key << "map" << YAML::Value << MapValue;
		Emitter << YAML::Key << "nested" << YAML::Value << Nested;
		Emitter << YAML::Key << "empty_list" << YAML::Value << NxFr::List<uint64>();
		Emitter << YAML::Key << "empty_map" << YAML::Value << NxFr::Dictionary<uint64, uint64>();

		NxFr::String Emitted = NxFr::Yaml::Serialize(Serialize);
		ASSERT_EQ(Emitted, Data);
	}

	TEST(Serialization, Rbs)
	{
		const char* CStringTest = "Nexus Test";
		NxFr::String StringTest = "StringTest";
		NxFr::String StringViewTest = "StringViewTest";
		NxFr::StringId StringIdTest = "StringIdTest"_Sid;

		NxFr::Array<NxFr::String> Array = NxFr::Array<NxFr::String>(5);
		Array[0] = "First";
		Array[1] = "Second";
		Array[2] = "Third";
		Array[3] = "Fourth";
		Array[4] = "Fifth";
		NxFr::List<NxFr::String> List = NxFr::List<NxFr::String>(5);
		List.AppendRange(Array);
		NxFr::Set<NxFr::String> Set = NxFr::Set<NxFr::String>(5);
		Set.AppendRange(Array);
		NxFr::Dictionary<NxFr::String, uint64> Dictionary = NxFr::Dictionary<NxFr::String, uint64>(5);
		for (uint64 Index = 0; Index < Array.GetCount(); ++Index)
		{
			Dictionary.Append(Array[Index], Index + 1);
		}

		NxFr::RBS RbsSerialization;
		RbsSerialization.WriteObject(true);
		RbsSerialization.WriteObject(1);
		RbsSerialization.WriteObject(3.14f);
		RbsSerialization.WriteObject((const char*)"Nexus Test");
		RbsSerialization.WriteObject(StringTest);
		RbsSerialization.WriteObject(StringViewTest);
		RbsSerialization.WriteObject(StringIdTest);
		RbsSerialization.WriteObject(YamlTestReference);
		RbsSerialization.WriteObject(Array);
		RbsSerialization.WriteObject(List);
		RbsSerialization.WriteObject(Set);
		RbsSerialization.WriteObject(Dictionary);
		NxFr::Buffer Serialization = NxFr::RBS::Serialize(RbsSerialization);

		NxFr::RBS RbsDeserialization = NxFr::RBS::Deserialize(Serialization);
		ASSERT_EQ(RbsDeserialization.ReadObject<bool>(), true);
		ASSERT_EQ(RbsDeserialization.ReadObject<int>(), 1);
		ASSERT_EQ(RbsDeserialization.ReadObject<float>(), 3.14f);
		ASSERT_EQ(RbsDeserialization.ReadObject<const char*>(), NxFr::StringView(CStringTest));
		ASSERT_EQ(RbsDeserialization.ReadObject<NxFr::String>(), StringTest);
		ASSERT_EQ(RbsDeserialization.ReadObject<NxFr::StringView>(), StringViewTest);
		ASSERT_EQ(RbsDeserialization.ReadObject<NxFr::StringId>(), StringIdTest);
		ASSERT_EQ(RbsDeserialization.ReadObject<SerializationTest>(), YamlTestReference);
		ASSERT_EQ(RbsDeserialization.ReadObject<NxFr::Array<NxFr::String>>()[4], "Fifth");
		ASSERT_EQ(RbsDeserialization.ReadObject<NxFr::List<NxFr::String>>()[4], "Fifth");
		ASSERT_EQ(RbsDeserialization.ReadObject<NxFr::Set<NxFr::String>>().Contains("Fifth"), true);
		auto Dict = RbsDeserialization.ReadObject<NxFr::Dictionary<NxFr::String, uint64>>();
		ASSERT_EQ(Dict["Fifth"], 5);
	}
}
