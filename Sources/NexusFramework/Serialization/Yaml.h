#pragma once

#include "NexusFramework/External/YamlCpp.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"

namespace NxFr
{
	namespace Yaml
	{
		// This wrap YAML::Node so it is guarenteed that yaml-cpp allocation are using the malloc / free and not Nexus allocators
		// The main reason is to avoid any DLL heap boundary issues,
		// as their new / delete operator are not overloaded to call Memory::Allocate / Memory::Free
		struct Node;

		struct NX_FRAMEWORK_API Iterator
		{
			Iterator(YAML::iterator It);

			Iterator& operator++();
			Iterator operator++(int);

			bool operator==(const Iterator& Other) const;
			bool operator!=(const Iterator& Other) const;

			Node Data();
			Node Key();
			Node Value();

			YAML::iterator It;
		};

		struct NX_FRAMEWORK_API ConstIterator
		{
			ConstIterator(YAML::const_iterator It);

			ConstIterator& operator++();
			ConstIterator operator++(int);

			bool operator==(const ConstIterator& Other) const;
			bool operator!=(const ConstIterator& Other) const;

			Node Data() const;
			Node Key() const;
			Node Value() const;

			YAML::const_iterator It;
		};

		struct NX_FRAMEWORK_API Node
		{
		public:
			Node()
				: Data()
			{
			}

			Node(const YAML::Node& Data)
				: Data(Data)
			{
			}

			~Node()
			{
				Allocator::Scope _ = nullptr;
				Data.~Node();
				new (&Data) YAML::Node();
			}

			template<typename T>
			Node operator[](const T& Key)
			{
				Allocator::Scope _ = nullptr;
				return Node(Data[Key]);
			}

			template<typename T>
			const Node operator[](const T& Key) const
			{
				Allocator::Scope _ = nullptr;
				return Node(Data[Key]);
			}

			template<typename T>
			Node& operator=(const T& Value)
			{
				Allocator::Scope _ = nullptr;
				Data = Value;
				return *this;
			}

			template<typename T>
			Node& Push(const T& Value)
			{
				Allocator::Scope _ = nullptr;
				Data.push_back<T>(Value);
				return *this;
			}

			template<typename T1, typename T2>
			Node& Insert(const T1& Key, const T2& Value)
			{
				Allocator::Scope _ = nullptr;
				Data.force_insert<T1, T2>(Key, Value);
				return *this;
			}

			Node& Remove(const Node& Key)
			{
				Allocator::Scope _ = nullptr;
				Data.remove(Key.Data);
				return *this;
			}

			template<typename T>
			T As() const
			{
				return Data.as<T>();
			}

			Node& SetTag(NxFr::StringView Tag)
			{
				Allocator::Scope _ = nullptr;
				Data.SetTag(Tag.C());
				return *this;
			}

			Node& SetStyle(YAML::EmitterStyle::value Style)
			{
				Allocator::Scope _ = nullptr;
				Data.SetStyle(Style);
				return *this;
			}

			Iterator Begin() { return Data.begin(); }
			ConstIterator Begin() const { return Data.begin(); }
			Iterator End() { return Data.end(); }
			ConstIterator End() const { return Data.end(); }

			YAML::NodeType::value GetType() const { return Data.Type(); }
			uint64 GetCount() const { return Data.size(); }

			YAML::Node Data;
		};
		
		inline constexpr uint64 SmallSequence = 10;

		NX_FRAMEWORK_API String Serialize(const Node& Data);
		NX_FRAMEWORK_API void SerializeAndSave(const Node& Data, StringView Path);
		NX_FRAMEWORK_API Node Deserialize(StringView Data);
		NX_FRAMEWORK_API Node LoadAndDeserialize(StringView Path);
	}
}

namespace YAML
{
	template<>
	struct convert<NxFr::Yaml::Node>
	{
		static Node encode(const NxFr::Yaml::Node& rhs)
		{
			return Node(rhs.Data);
		}

		static bool decode(const Node& node, NxFr::Yaml::Node& rhs)
		{
			rhs = node;
			return true;
		}
	};
}
