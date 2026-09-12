#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Yaml.h"

#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	namespace Yaml
	{
		Iterator::Iterator(YAML::iterator It)
			: It(It)
		{
		}

		Iterator& Iterator::operator++()
		{
			++It;
			return *this;
		}

		Iterator Iterator::operator++(int)
		{
			Iterator Return(It);
			++It;
			return Return;
		}

		bool Iterator::operator==(const Iterator& Other) const
		{
			return It == Other.It;
		}

		bool Iterator::operator!=(const Iterator& Other) const
		{
			return It != Other.It;
		}

		Node Iterator::Data()
		{
			return *It;
		}

		Node Iterator::Key()
		{
			return It->first;
		}

		Node Iterator::Value()
		{
			return It->second;
		}

		ConstIterator::ConstIterator(YAML::const_iterator It)
			: It(It)
		{
		}

		ConstIterator& ConstIterator::operator++()
		{
			++It;
			return *this;
		}

		ConstIterator ConstIterator::operator++(int)
		{
			ConstIterator Return(It);
			++It;
			return Return;
		}

		bool ConstIterator::operator==(const ConstIterator& Other) const
		{
			return It == Other.It;
		}

		bool ConstIterator::operator!=(const ConstIterator& Other) const
		{
			return It != Other.It;
		}

		Node ConstIterator::Data() const
		{
			return *It;
		}

		Node ConstIterator::Key() const
		{
			return It->first;
		}

		Node ConstIterator::Value() const
		{
			return It->second;
		}

		String Serialize(const Node& Data)
		{
			Allocator::Scope _ = nullptr;
			return YAML::Dump(Data.Data).c_str();
		}

		void SerializeAndSave(const Node& Data, StringView Path)
		{
			TextStream Stream(Path);
			Stream.Open(File::Mode::Write);
			Stream.WriteAll(Serialize(Data));
			Stream.Close();
		}

		Node Deserialize(StringView Data)
		{
			Allocator::Scope _ = nullptr;
			return YAML::Load(Data.C());
		}

		Node LoadAndDeserialize(StringView Path)
		{
			Allocator::Scope _ = nullptr;
			return YAML::LoadFile(Path.C());
		}
	}
}
