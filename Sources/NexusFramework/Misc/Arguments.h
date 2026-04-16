#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"

#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Collection.h"

namespace NxFr
{
	namespace Arguments
	{
		NEXUS_FRAMEWORK_API const Array<String>& GetArguments();
		NEXUS_FRAMEWORK_API const List<String>& GetPositionals();
		NEXUS_FRAMEWORK_API const Dictionary<String, String>& GetNamed();

		NEXUS_FRAMEWORK_API StringView Get(uint64 Index, StringView Default = "");
		NEXUS_FRAMEWORK_API StringView Get(StringView Key, StringView Default = "");
		NEXUS_FRAMEWORK_API bool Has(uint64 Index);
		NEXUS_FRAMEWORK_API bool Has(StringView Key);

		NEXUS_FRAMEWORK_API void Parse(uint64 ArgC, char* ArgV[]);

		NEXUS_FRAMEWORK_API void Parse(const Collection<StringView>& Args, Array<String>& Arguments, List<String>& Positionals, Dictionary<String, String>& Named);
		NEXUS_FRAMEWORK_API void Parse(StringView Command, Array<String>& Arguments, List<String>& Positionals, Dictionary<String, String>& Named);
	}
}
