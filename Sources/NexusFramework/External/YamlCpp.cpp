#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/External/YamlCpp.h"

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
	}
}
