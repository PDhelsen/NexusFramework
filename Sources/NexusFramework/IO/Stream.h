#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API Stream
	{
	public:
		inline static constexpr uint64 BlockSize = 1024;

		NEXUS_NOCOPY(Stream)
		Stream(StringView Path);
		Stream(Stream&& Other) noexcept;
		virtual ~Stream();

		Stream& operator=(Stream&& Other) noexcept;

		void Open(File::Mode Mode, bool CreateIfDontExist = true);
		void Close();

		virtual void Cache() = 0;
		virtual void Flush() = 0;

		virtual uint64 GetSize() const = 0;
		virtual uint64 GetCursor() const { return Cursor; }
		virtual void SetCursor(uint64 Position) { Cursor = Position; }
		virtual bool IsAtTheEnd() const { return Cursor == GetSize(); }

		StringView GetPath() const { return Handle.GetPath(); }
		File::Mode GetMode() const { return Handle.GetMode(); }
		bool IsOpened() const { return Handle.IsOpened(); }

	protected:
		File Handle;
		uint64 Cursor;
	};

	class NEXUS_FRAMEWORK_API TextStream : public Stream
	{
	public:
		NEXUS_NOCOPY(TextStream)
		TextStream(StringView Path);
		TextStream(TextStream&& Other) noexcept;
		virtual ~TextStream();

		TextStream& operator=(TextStream&& Other) noexcept;

		void Cache() override;
		void Flush() override;

		StringView ReadAll();
		StringView ReadBlock(uint64 Size);
		StringView ReadLine();
		void WriteAll(StringView Data);
		void WriteBlock(StringView Data);
		void WriteLine(StringView Data);

		uint64 GetSize() const { return Buffer.GetCount(); };

	private:
		String Buffer;
	};

	class NEXUS_FRAMEWORK_API BinaryStream : public Stream
	{
	public:
		NEXUS_NOCOPY(BinaryStream)
		BinaryStream(StringView Path);
		BinaryStream(BinaryStream&& Other) noexcept;
		virtual ~BinaryStream();

		BinaryStream& operator=(BinaryStream&& Other) noexcept;

		void Cache() override;
		void Flush() override;

		BufferView ReadAll();
		BufferView ReadBlock(uint64 Size);
		void WriteAll(BufferView Data);
		void WriteBlock(BufferView Data);

		uint64 GetSize() const { return Buffer.GetCount(); };

	private:
		Buffer Buffer;
	};
}
