#pragma once

#include "Core/NexusEngine.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Buffer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"

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

		NEXUS_ENGINE_API void Create(bool KeepOpen = false);
		NEXUS_ENGINE_API void Move(StringView Target, bool Override = false, bool CloseIfOpen = false);
		NEXUS_ENGINE_API void Copy(StringView Target, bool Override = false, bool CloseIfOpen = false);
		NEXUS_ENGINE_API void Delete(bool CloseIfOpen = false);

		NEXUS_ENGINE_API void Open(Mode OpenMode, bool CreateIfDontExist = false);
		NEXUS_ENGINE_API void Close();

		NEXUS_ENGINE_API uint64 GetSize();
		NEXUS_ENGINE_API void WriteByte(BufferView<Byte> Data);
		NEXUS_ENGINE_API Buffer<Byte> ReadByte(Allocator* Allctr = nullptr);

		StringView GetPath() const { return Path.ToView(); }
		bool Exists() const { return Exist; }
		bool IsOpened() const { return Handle != nullptr; }

	private:
		String Path;
		bool Exist;
		void* Handle;
	};
}
