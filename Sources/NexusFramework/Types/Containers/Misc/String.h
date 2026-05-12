#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/Utility.h"

#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dequeue.h"
#include "NexusFramework/Types/Containers/Queue.h"
#include "NexusFramework/Types/Containers/Stack.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/LinkedList.h"
#include "NexusFramework/Types/Containers/Tree.h"
#include "NexusFramework/Types/Containers/Graph.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Types/Containers/Tuple.h"
#include "NexusFramework/Types/Containers/Collection.h"
#include "NexusFramework/Types/Containers/Utility.h"

namespace NxFr
{
	template<typename T>
	struct StringConverter<Array<T>>
	{
		static void ToString(const Array<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Array<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Array");
		}
	};

	template<typename T>
	struct StringConverter<List<T>>
	{
		static void ToString(const List<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, List<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with List");
		}
	};

	template<typename T>
	struct StringConverter<Dequeue<T>>
	{
		static void ToString(const Dequeue<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Dequeue<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Dequeue");
		}
	};

	template<typename T>
	struct StringConverter<Stack<T>>
	{
		static void ToString(const Stack<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Stack<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Stack");
		}
	};

	template<typename T>
	struct StringConverter<Queue<T>>
	{
		static void ToString(const Queue<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Queue<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Queue");
		}
	};

	template<typename T>
	struct StringConverter<LinkedList<T>>
	{
		static void ToString(const LinkedList<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, LinkedList<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with LinkedList");
		}
	};

	template<typename T>
	struct StringConverter<Set<T>>
	{
		static void ToString(const Set<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<T>(It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Set<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Set");
		}
	};

	template<typename K, typename T>
	struct StringConverter<Dictionary<K, T>>
	{
		static void ToString(const Dictionary<K, T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& [Key, Value] : Data)
			{
				Result += "Key: ";
				Result += StringUtility::ToString<K>(Key, Format);
				Result += " => ";
				Result += "Value: ";
				Result += StringUtility::ToString<T>(Value, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Dictionary<K, T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Dictionary");
		}
	};

	template<>
	struct StringConverter<BufferView>
	{
		static void ToString(const BufferView& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (const auto& It : Data)
			{
				Result += StringUtility::ToString<uint8>(It, Format);
			}
		}

		static void FromString(StringView Data, BufferView& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Tuple");
		}
	};

	template<typename T1, typename T2>
	struct StringConverter<Tuple<T1, T2>>
	{
		static void ToString(const Tuple<T1, T2>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			Result += StringUtility::ToString<T1>(Data.GetFirst(), Format);
			Result += StringUtility::ToString<T2>(Data.GetSecond(), Format);
		}

		static void FromString(StringView Data, Tuple<T1, T2>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Tuple");
		}
	};

	template<typename T>
	struct StringConverter<Collection<T>>
	{
		static void ToString(const Collection<T>& Data, String& Result, StringView Format = "")
		{
			Result.Clear();
			for (auto It = Data.Begin(); It != Data.End(); ++It)
			{
				Result += StringUtility::ToString<T>(*It, Format);
				Result += StringUtility::NewLine;
			}
		}

		static void FromString(StringView Data, Collection<T>& Result, StringView Format = "")
		{
			NEXUS_ASSERT(false, Default, "Unsupported FromString with Collection");
		}
	};
}
