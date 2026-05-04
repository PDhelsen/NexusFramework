#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	Stream::Stream(StringView Path)
		: Handle(Path), Cursor(0)
	{
	}

	Stream::Stream(Stream&& Other) noexcept
		: Handle(Move(Other.Handle)), Cursor(Other.Cursor)
	{
	}

	Stream::~Stream()
	{
	}

	Stream& Stream::operator=(Stream&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Handle = Move(Other.Handle);
		Cursor = Other.Cursor;

		return *this;
	}

	void Stream::Open(File::Mode Mode, bool CreateIfDontExist)
	{
		Cursor = 0;
		Handle.Open(Mode, CreateIfDontExist);

		if (Handle.GetMode() == File::Mode::Read)
		{
			Cache();
		}
	}

	void Stream::Close()
	{
		if (Handle.GetMode() == File::Mode::Write || Handle.GetMode() == File::Mode::Append)
		{
			Flush();
		}

		Handle.Close();
	}

	TextStream::TextStream(StringView Path)
		: Stream(Path), Buffer(BlockSize)
	{
	}

	TextStream::TextStream(TextStream&& Other) noexcept
		: Stream(Move(Other)), Buffer(Move(Other.Buffer))
	{
	}

	TextStream::~TextStream()
	{
	}

	TextStream& TextStream::operator=(TextStream&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Stream::operator=(Move(Other));
		Buffer = Move(Other.Buffer);

		return *this;
	}

	void TextStream::Cache()
	{
		Buffer.Clear();
		if (IsOpened())
		{
			Buffer = Handle.ReadText();
		}
	}

	void TextStream::Flush()
	{
		if (IsOpened())
		{
			Handle.WriteText(Buffer);
		}
		Buffer.Clear();
	}

	StringView TextStream::ReadAll()
	{
		StringView View = Buffer.Substring(Cursor, Buffer.GetCount() - Cursor);
		Cursor = Buffer.GetCount();
		return View;
	}

	StringView TextStream::ReadBlock(uint64 Size)
	{
		StringView Substring = Buffer.Substring(Cursor, Size);
		Cursor = Math::Min(Cursor + Size, Buffer.GetCount());
		return Substring;
	}

	StringView TextStream::ReadLine()
	{
		Iterator::StringToken Iterator = Iterator::StringToken(StringUtility::NewLine, Buffer, Cursor);
		StringView Substring = Iterator.Get();
		Cursor = Math::Min((++Iterator).Id(), Buffer.GetCount());
		return Substring;
	}

	void TextStream::WriteAll(StringView Data)
	{
		Buffer.Clear();
		Buffer.Append(Data);
		Cursor = Buffer.GetCount();
	}

	void TextStream::WriteBlock(StringView Data)
	{
		Buffer.Append(Data);
		Cursor += Data.GetCount();
	}

	void TextStream::WriteLine(StringView Data)
	{
		Buffer.Append(Data);
		Buffer.Append(StringUtility::NewLine);
		Cursor += Data.GetCount() + StringUtility::NewLine.GetCount();
	}

	BinaryStream::BinaryStream(StringView Path)
		: Stream(Path), Buffer(BlockSize)
	{
	}

	BinaryStream::BinaryStream(BinaryStream&& Other) noexcept
		: Stream(Move(Other)), Buffer(Move(Other.Buffer))
	{
	}

	BinaryStream::~BinaryStream()
	{
	}

	BinaryStream& BinaryStream::operator=(BinaryStream&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Stream::operator=(Move(Other));
		Buffer = Move(Other.Buffer);

		return *this;
	}

	void BinaryStream::Cache()
	{
		Buffer.Clear();
		if (IsOpened())
		{
			Buffer = Handle.ReadByte();
		}
	}

	void BinaryStream::Flush()
	{
		if (IsOpened())
		{
			Handle.WriteByte(Buffer);
		}
		Buffer.Clear();
	}

	BufferView BinaryStream::ReadAll()
	{
		BufferView View = Buffer.Get(Cursor, Buffer.GetCount() - Cursor);
		Cursor = Buffer.GetCount();
		return View;
	}

	BufferView BinaryStream::ReadBlock(uint64 Size)
	{
		BufferView View = Buffer.Get(Size, Cursor);
		Cursor = Math::Min(Cursor + Size, Buffer.GetCount());
		return View;
	}

	void BinaryStream::WriteAll(BufferView Data)
	{
		if (Data.GetCount() > Buffer.GetCount())
		{
			Buffer.Resize(Data.GetCount());
		}

		Buffer.Clear();
		Buffer.Set(Data.GetPtr(), Data.GetCount());
		Cursor = Data.GetCount();
	}

	void BinaryStream::WriteBlock(BufferView Data)
	{
		if (Cursor + Data.GetCount() > Buffer.GetCount())
		{
			Buffer.Grow(Math::Max(BlockSize, Data.GetCount()));
		}

		Buffer.Set(Data.GetPtr(), Data.GetCount(), Cursor);
		Cursor += Data.GetCount();
	}
}


