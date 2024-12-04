#pragma once

#include "Core/NexusEngine.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Containers/List.h"

namespace NxEn
{
	class File
	{
	public:
		NEXUS_ENGINE_API File(StringView Path);
		NEXUS_ENGINE_API File(String&& Path);

		NEXUS_ENGINE_API explicit operator bool() const;
		NEXUS_ENGINE_API bool operator ==(const File& Other) const;
		NEXUS_ENGINE_API bool operator !=(const File& Other) const;

		NEXUS_ENGINE_API void Refresh();

		NEXUS_ENGINE_API bool Create();
		NEXUS_ENGINE_API bool Move(StringView Target);
		NEXUS_ENGINE_API bool Delete();

		StringView GetPath() const { return Path.ToView(); }
		bool Exists() const { return Exist; }

	private:
		String Path;
		bool Exist;
	};
}
