#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	namespace Arguments
	{
		inline static const StringView KeyProgram = "Program";

		NEXUS_FRAMEWORK_API void Parse(uint64 ArgC, char* ArgV[]);
		NEXUS_FRAMEWORK_API void Log();

		NEXUS_FRAMEWORK_API StringView GetValue(uint64 Index, StringView Default = "");
		NEXUS_FRAMEWORK_API StringView GetValue(StringView Key, StringView Default = "");
		NEXUS_FRAMEWORK_API bool HasFlag(StringView Key);

		NEXUS_FRAMEWORK_API Dictionary<StringView, StringView>::I Begin();
		NEXUS_FRAMEWORK_API Dictionary<StringView, StringView>::I End();
		NEXUS_FRAMEWORK_API int64 GetCount();
	}
}
