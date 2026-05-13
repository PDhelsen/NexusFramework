#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Containers/List.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API Directory
	{
	public:
		Directory(StringView Path);

		explicit operator bool() const;
		bool operator ==(const Directory& Other) const;
		bool operator !=(const Directory& Other) const;

		bool Exists() const;
		void EnsureParent() const;

		void Create();
		void Move(StringView Target, bool Override = false);
		void Copy(StringView Target, bool Override = false);
		void Delete();

		List<String> GetContent(bool Recursive = false) const;
		void GetContent(List<String>& Result, bool Recursive = false) const;
		List<String> GetFiles(bool Recursive = false) const;
		void GetFiles(List<String>& Result, bool Recursive = false) const;
		List<String> GetDirectories(bool Recursive = false) const;
		void GetDirectories(List<String>& Result, bool Recursive = false) const;

		StringView GetPath() const { return Path; }

	private:
		void SetPath(StringView Value);
		void GetContent(List<String>& Result, bool Recursive, bool GetFile, bool GetDirectory) const;

		String Path;
	};
}
