#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Strings/String.h"

namespace NxFr
{
	class Directory
	{
	public:
		NEXUS_FRAMEWORK_API Directory(StringView Path);

		NEXUS_FRAMEWORK_API explicit operator bool() const;
		NEXUS_FRAMEWORK_API bool operator ==(const Directory& Other) const;
		NEXUS_FRAMEWORK_API bool operator !=(const Directory& Other) const;

		NEXUS_FRAMEWORK_API bool Exists() const;
		NEXUS_FRAMEWORK_API void EnsureParent() const;

		NEXUS_FRAMEWORK_API void Create();
		NEXUS_FRAMEWORK_API void Move(StringView Target, bool Override = false);
		NEXUS_FRAMEWORK_API void Copy(StringView Target, bool Override = false);
		NEXUS_FRAMEWORK_API void Delete();

		NEXUS_FRAMEWORK_API List<String> GetContent(bool Recursive = false) const;
		NEXUS_FRAMEWORK_API void GetContent(List<String>& Result, bool Recursive = false) const;
		NEXUS_FRAMEWORK_API List<String> GetFiles(bool Recursive = false) const;
		NEXUS_FRAMEWORK_API void GetFiles(List<String>& Result, bool Recursive = false) const;
		NEXUS_FRAMEWORK_API List<String> GetDirectories(bool Recursive = false) const;
		NEXUS_FRAMEWORK_API void GetDirectories(List<String>& Result, bool Recursive = false) const;

		StringView GetPath() const { return Path; }

	private:
		void SetPath(StringView Value);
		void GetContent(List<String>& Result, bool Recursive, bool GetFile, bool GetDirectory) const;

		String Path;
	};
}
