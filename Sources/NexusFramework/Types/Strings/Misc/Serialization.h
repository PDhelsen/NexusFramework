#pragma once

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Strings/StringCApi.h"

#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	namespace RBSUtility
	{
		template<>
		struct Converter<const char*>
		{
			static const char* Decode(const RBS& Rbs)
			{
				uint64 Size = Rbs.ReadObject<uint64>();
				const char* Text = Rbs.ReadData<char>(Size);
				Rbs.ReadByte(sizeof(StringCApi::NullChar));
				return Text;
			}

			static void Encode(RBS& Rbs, const char* Object)
			{
				uint64 Size = StringCApi::Length(Object);

				Rbs.WriteObject(Size);
				Rbs.WriteData(Object, StringCApi::Length(Object));
				Rbs.WriteObject(StringCApi::NullChar);
			}
		};

		template<size_t N>
		struct Converter<char[N]>
		{
			static const char* Decode(const RBS& Rbs)
			{
				uint64 Size = Rbs.ReadObject<uint64>();
				const char* Text = Rbs.ReadData<char>(Size);
				Rbs.ReadByte(sizeof(StringCApi::NullChar));
				return Text;
			}

			static void Encode(RBS& Rbs, const char(&Object)[N])
			{
				uint64 Size = StringCApi::Length(Object);

				Rbs.WriteObject(Size);
				Rbs.WriteData(Object, StringCApi::Length(Object));
				Rbs.WriteObject(StringCApi::NullChar);
			}
		};

		template<>
		struct Converter<String>
		{
			static String Decode(const RBS& Rbs)
			{
				return Rbs.ReadObject<const char*>();
			}

			static void Encode(RBS& Rbs, const String& Object)
			{
				Rbs.WriteObject<const char*>(Object.C());
			}
		};

		template<>
		struct Converter<StringView>
		{
			static StringView Decode(const RBS& Rbs)
			{
				return Rbs.ReadObject<const char*>();
			}

			static void Encode(RBS& Rbs, const StringView& Object)
			{
				Rbs.WriteObject<const char*>(Object.C(true));
			}
		};

		template<>
		struct Converter<StringId>
		{
			static StringId Decode(const RBS& Rbs)
			{
				return Rbs.ReadObject<StringView>();
			}

			static void Encode(RBS& Rbs, const StringId& Object)
			{
				Rbs.WriteObject<StringView>(Object);
			}
		};
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
			rhs = node.as<std::string_view>().data();
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
			return Node(rhs.GetString());
		}

		static bool decode(const Node& node, NxFr::StringId& rhs)
		{
			if (!node.IsScalar())
			{
				return false;
			}

			rhs = NxFr::StringId(node.as<NxFr::String>());
			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::StringId& rhs)
	{
		out << rhs.GetString();
		return out;
	}
}
