#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	class Directory
	{
	public:
		using I = List<String>::I;

		NEXUS_FRAMEWORK_API Directory(StringView Path);
		NEXUS_FRAMEWORK_API ~Directory();

		NEXUS_FRAMEWORK_API explicit operator bool() const;
		NEXUS_FRAMEWORK_API bool operator ==(const Directory& Other) const;
		NEXUS_FRAMEWORK_API bool operator !=(const Directory& Other) const;

		NEXUS_FRAMEWORK_API Directory& Refresh();
		NEXUS_FRAMEWORK_API Directory& EnsureParent();

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

		I begin() { return Begin(); }
		I Begin() { return Content.Begin(); }
		const I begin() const { return Begin(); }
		const I Begin() const { return Content.Begin(); }
		I end() { return End(); }
		I End() { return Content.End(); }
		const I end() const { return End(); }
		const I End() const { return Content.End(); }

		StringView GetPath() const { return Path.ToView(); }
		bool Exists() const { return Exist; }
		uint64 GetCount() const { return Content.GetCount(); }

	private:
		String Path;
		bool Exist;
		List<String> Content;
	};
}
