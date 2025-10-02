#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/String.h"

namespace NxFr
{
	namespace Arguments
	{
		NEXUS_FRAMEWORK_API void Parse(uint64 ArgC, char* ArgV[]);
		NEXUS_FRAMEWORK_API void Log();

		NEXUS_FRAMEWORK_API StringView Get(uint64 Index, StringView Default = "");
		NEXUS_FRAMEWORK_API StringView Get(StringView Key, StringView Default = "");
		NEXUS_FRAMEWORK_API bool Has(uint64 Index);
		NEXUS_FRAMEWORK_API bool Has(StringView Key);
	}
}
