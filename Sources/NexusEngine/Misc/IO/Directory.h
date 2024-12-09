#pragma once

#include "Core/NexusEngine.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Containers/List.h"

namespace NxEn
{
	class Directory
	{
	public:
		using I = List<String>::I;

		NEXUS_ENGINE_API Directory(StringView Path);
		NEXUS_ENGINE_API Directory(String&& Path);

		NEXUS_ENGINE_API explicit operator bool() const;
		NEXUS_ENGINE_API bool operator ==(const Directory& Other) const;
		NEXUS_ENGINE_API bool operator !=(const Directory& Other) const;

		NEXUS_ENGINE_API void Refresh();

		NEXUS_ENGINE_API bool Create();
		NEXUS_ENGINE_API bool Move(StringView Target);
		NEXUS_ENGINE_API bool Delete();

		NEXUS_ENGINE_API List<String> GetContent(bool Recursive = false) const;
		NEXUS_ENGINE_API void GetContent(List<String>& Result, bool Recursive = false) const;
		NEXUS_ENGINE_API List<String> GetFiles(bool Recursive = false) const;
		NEXUS_ENGINE_API void GetFiles(List<String>& Result, bool Recursive = false) const;
		NEXUS_ENGINE_API List<String> GetDirectories(bool Recursive = false) const;
		NEXUS_ENGINE_API void GetDirectories(List<String>& Result, bool Recursive = false) const;

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
