#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	namespace Arguments
	{
		NEXUS_FRAMEWORK_API void Parse(uint64 ArgC, char* ArgV[]);

		NEXUS_FRAMEWORK_API bool HasFlag(StringView Key, bool Default = false);
		NEXUS_FRAMEWORK_API StringView GetValue(StringView Key, StringView Default = "");

		NEXUS_FRAMEWORK_API int64 GetCount();
		NEXUS_FRAMEWORK_API StringView GetProgram();

		NEXUS_FRAMEWORK_API void Log();
	}
}
