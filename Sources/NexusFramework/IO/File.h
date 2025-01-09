#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Types/Containers/Buffer.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxFr
{
	class File
	{
	public:
		enum class Mode
		{
			Read, Write, Append
		};

		NEXUS_FRAMEWORK_API File(StringView Path);
		NEXUS_FRAMEWORK_API ~File();

		NEXUS_FRAMEWORK_API explicit operator bool() const;
		NEXUS_FRAMEWORK_API bool operator ==(const File& Other) const;
		NEXUS_FRAMEWORK_API bool operator !=(const File& Other) const;

		NEXUS_FRAMEWORK_API File& Refresh();

		NEXUS_FRAMEWORK_API void Create(bool KeepOpen = false);
		NEXUS_FRAMEWORK_API void Move(StringView Target, bool Override = false, bool CloseIfOpen = false);
		NEXUS_FRAMEWORK_API void Copy(StringView Target, bool Override = false, bool CloseIfOpen = false);
		NEXUS_FRAMEWORK_API void Delete(bool CloseIfOpen = false);

		NEXUS_FRAMEWORK_API void Open(Mode OpenMode, bool CreateIfDontExist = false);
		NEXUS_FRAMEWORK_API void Close();

		NEXUS_FRAMEWORK_API uint64 GetSize();
		NEXUS_FRAMEWORK_API void WriteByte(BufferView<Byte> Data);
		NEXUS_FRAMEWORK_API Buffer<Byte> ReadByte();
		NEXUS_FRAMEWORK_API void WriteText(StringView Text);
		NEXUS_FRAMEWORK_API String ReadText();

		StringView GetPath() const { return Path.ToView(); }
		bool Exists() const { return Exist; }
		bool IsOpened() const { return Handle != nullptr; }

	private:
		String Path;
		bool Exist;
		void* Handle;
	};
}
