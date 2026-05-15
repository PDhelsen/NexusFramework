#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API File
	{
	public:
		enum class Mode
		{
			None, Read, Write, Append
		};

		NEXUS_NOCOPY(File)
		File(StringView Path);
		File(File&& Other) noexcept;
		~File();

		File& operator=(File&& Other) noexcept;
		explicit operator bool() const;
		bool operator ==(const File& Other) const;
		bool operator !=(const File& Other) const;

		bool Exists() const;
		void EnsureParent() const;

		void Create(bool KeepOpen = false);
		void Move(StringView Target, bool Override = false, bool CloseIfOpen = false);
		void Copy(StringView Target, bool Override = false, bool CloseIfOpen = false);
		void Delete(bool CloseIfOpen = false);

		void Open(Mode OpenMode, bool CreateIfDontExist = false);
		void Close();

		uint64 GetSize() const;
		void WriteByte(BufferView Data);
		Buffer ReadByte() const;
		void WriteText(StringView Text);
		String ReadText() const;

		StringView GetPath() const { return Path; }
		File::Mode GetMode() const { return FileMode; }
		bool IsOpened() const { return Handle != nullptr; }

	private:
		void SetPath(StringView Value);

		String Path;
		Mode FileMode;
		void* Handle;
	};
}
