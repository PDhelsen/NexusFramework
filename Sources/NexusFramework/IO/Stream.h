#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	class Stream
	{
	public:
		inline static constexpr uint64 BlockSize = 1024;

		NEXUS_FRAMEWORK_API Stream(StringView Path);
		NEXUS_FRAMEWORK_API Stream(const Stream& Other) = delete;
		NEXUS_FRAMEWORK_API Stream(Stream&& Other) noexcept;
		NEXUS_FRAMEWORK_API virtual ~Stream();

		NEXUS_FRAMEWORK_API Stream& operator=(const Stream& Other) = delete;
		NEXUS_FRAMEWORK_API Stream& operator=(Stream&& Other) noexcept;

		NEXUS_FRAMEWORK_API void Open(File::Mode Mode, bool CreateIfDontExist = true);
		NEXUS_FRAMEWORK_API void Close();

		NEXUS_FRAMEWORK_API virtual void Cache() = 0;
		NEXUS_FRAMEWORK_API virtual void Flush() = 0;

		NEXUS_FRAMEWORK_API virtual uint64 GetSize() const = 0;
		NEXUS_FRAMEWORK_API virtual uint64 GetCursor() const { return Cursor; }
		NEXUS_FRAMEWORK_API virtual void SetCursor(uint64 Position) { Cursor = Position; }
		NEXUS_FRAMEWORK_API virtual bool IsAtTheEnd() const { return Cursor == GetSize(); }

		NEXUS_FRAMEWORK_API StringView GetPath() const { return Handle.GetPath(); }
		NEXUS_FRAMEWORK_API File::Mode GetMode() const { return Handle.GetMode(); }
		NEXUS_FRAMEWORK_API bool IsOpened() const { return Handle.IsOpened(); }

	protected:
		File Handle;
		uint64 Cursor;
	};

	class TextStream : public Stream
	{
	public:
		NEXUS_FRAMEWORK_API TextStream(StringView Path);
		NEXUS_FRAMEWORK_API TextStream(const TextStream& Other) = delete;
		NEXUS_FRAMEWORK_API TextStream(TextStream&& Other) noexcept;
		NEXUS_FRAMEWORK_API virtual ~TextStream();

		NEXUS_FRAMEWORK_API TextStream& operator=(const TextStream& Other) = delete;
		NEXUS_FRAMEWORK_API TextStream& operator=(TextStream&& Other) noexcept;

		NEXUS_FRAMEWORK_API void Cache() override;
		NEXUS_FRAMEWORK_API void Flush() override;

		NEXUS_FRAMEWORK_API StringView ReadAll();
		NEXUS_FRAMEWORK_API StringView ReadBlock(uint64 Size);
		NEXUS_FRAMEWORK_API StringView ReadLine();
		NEXUS_FRAMEWORK_API void WriteAll(StringView Data);
		NEXUS_FRAMEWORK_API void WriteBlock(StringView Data);
		NEXUS_FRAMEWORK_API void WriteLine(StringView Data);

		NEXUS_FRAMEWORK_API uint64 GetSize() const { return Buffer.GetCount(); };

	private:
		String Buffer;
	};

	class BinaryStream : public Stream
	{
	public:
		NEXUS_FRAMEWORK_API BinaryStream(StringView Path);
		NEXUS_FRAMEWORK_API BinaryStream(const BinaryStream& Other) = delete;
		NEXUS_FRAMEWORK_API BinaryStream(BinaryStream&& Other) noexcept;
		NEXUS_FRAMEWORK_API virtual ~BinaryStream();

		NEXUS_FRAMEWORK_API BinaryStream& operator=(const BinaryStream& Other) = delete;
		NEXUS_FRAMEWORK_API BinaryStream& operator=(BinaryStream&& Other) noexcept;

		NEXUS_FRAMEWORK_API void Cache() override;
		NEXUS_FRAMEWORK_API void Flush() override;

		NEXUS_FRAMEWORK_API BufferView ReadAll();
		NEXUS_FRAMEWORK_API BufferView ReadBlock(uint64 Size);
		NEXUS_FRAMEWORK_API void WriteAll(BufferView Data);
		NEXUS_FRAMEWORK_API void WriteBlock(BufferView Data);

		NEXUS_FRAMEWORK_API uint64 GetSize() const { return Buffer.GetCount(); };

	private:
		Buffer Buffer;
	};
}
