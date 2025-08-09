#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	class Stream
	{
	public:
		const uint64 BlockSize = 1024;

		NEXUS_FRAMEWORK_API Stream(StringView Path);
		NEXUS_FRAMEWORK_API virtual ~Stream();

		NEXUS_FRAMEWORK_API void Open(File::Mode Mode, bool CreateIfDontExist = true);
		NEXUS_FRAMEWORK_API void Close();

		NEXUS_FRAMEWORK_API virtual bool IsAtTheEnd() = 0;
		NEXUS_FRAMEWORK_API virtual void Cache() = 0;
		NEXUS_FRAMEWORK_API virtual void Flush() = 0;

		NEXUS_FRAMEWORK_API StringView GetPath() const { return Handle.GetPath(); }
		NEXUS_FRAMEWORK_API const File& GetFile() const { return Handle; }
		NEXUS_FRAMEWORK_API File::Mode GetMode() const { return FileMode; }
		NEXUS_FRAMEWORK_API bool IsOpened() const { return Handle.IsOpened(); }

	protected:
		File Handle;
		File::Mode FileMode;
	};

	class TextStream : public Stream
	{
	public:
		NEXUS_FRAMEWORK_API TextStream(StringView Path);
		NEXUS_FRAMEWORK_API virtual ~TextStream();

		NEXUS_FRAMEWORK_API bool IsAtTheEnd() override;
		NEXUS_FRAMEWORK_API void Cache() override;
		NEXUS_FRAMEWORK_API void Flush() override;

		NEXUS_FRAMEWORK_API StringView Read();
		NEXUS_FRAMEWORK_API void Write(StringView Data);

	private:
		String Buffer;
		uint64 Cursor;
	};

	class BinaryStream : public Stream
	{
	public:
		NEXUS_FRAMEWORK_API BinaryStream(StringView Path);
		NEXUS_FRAMEWORK_API virtual ~BinaryStream();

		NEXUS_FRAMEWORK_API bool IsAtTheEnd() override;
		NEXUS_FRAMEWORK_API void Cache() override;
		NEXUS_FRAMEWORK_API void Flush() override;

		NEXUS_FRAMEWORK_API BufferView Read(uint64 Size);
		NEXUS_FRAMEWORK_API void Write(BufferView Data);

	private:
		Buffer Buffer;
		uint64 Cursor;
	};
}
