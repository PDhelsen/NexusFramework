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

		NEXUS_FRAMEWORK_API const String& Get(uint64 Index, const String& Default = "");
		NEXUS_FRAMEWORK_API const String& Get(const String& Key, const String& Default = "");
		NEXUS_FRAMEWORK_API bool Has(uint64 Index);
		NEXUS_FRAMEWORK_API bool Has(const String& Key);
	}
}
