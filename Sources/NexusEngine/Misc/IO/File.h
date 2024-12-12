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
		enum class Mode
		{
			Read, Write, Append
		};

		NEXUS_ENGINE_API File(StringView Path);
		NEXUS_ENGINE_API File(String&& Path);
		NEXUS_ENGINE_API ~File();

		NEXUS_ENGINE_API explicit operator bool() const;
		NEXUS_ENGINE_API bool operator ==(const File& Other) const;
		NEXUS_ENGINE_API bool operator !=(const File& Other) const;

		NEXUS_ENGINE_API File& Refresh();

		NEXUS_ENGINE_API bool Create(bool KeepOpen = false);
		NEXUS_ENGINE_API bool Move(StringView Target, bool Override = false, bool CloseIfOpen = false);
		NEXUS_ENGINE_API bool Copy(StringView Target, bool Override = false, bool CloseIfOpen = false);
		NEXUS_ENGINE_API bool Delete(bool CloseIfOpen = false);

		NEXUS_ENGINE_API bool Open(Mode OpenMode, bool CreateIfDontExist = false);
		NEXUS_ENGINE_API bool Close();

		NEXUS_ENGINE_API bool Write(void* Data, uint64 Size);

		StringView GetPath() const { return Path.ToView(); }
		bool Exists() const { return Exist; }
		bool IsOpened() const { return Handle != nullptr; }

	private:
		String Path;
		bool Exist;
		void* Handle;
	};
}
