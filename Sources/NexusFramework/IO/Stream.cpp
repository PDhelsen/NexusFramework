#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	Stream::Stream(StringView Path)
		: Handle(Path), FileMode(File::Mode::Read)
	{
	}

	Stream::~Stream()
	{
	}

	void Stream::Open(File::Mode Mode, bool CreateIfDontExist)
	{
		FileMode = Mode;
		Handle.Open(Mode, CreateIfDontExist);

		if (FileMode == File::Mode::Read)
		{
			Cache();
		}
	}

	void Stream::Close()
	{
		if (FileMode == File::Mode::Write || FileMode == File::Mode::Append)
		{
			Flush();
		}

		Handle.Close();
	}

	TextStream::TextStream(StringView Path)
		: Stream(Path), Buffer(BlockSize), Cursor()
	{
	}

	TextStream::~TextStream()
	{
			
	}

	bool TextStream::IsAtTheEnd()
	{
		return Cursor == Buffer.GetCount();
	}

	void TextStream::Cache()
	{
		Buffer = Handle.ReadText();
		Cursor = 0;
	}

	void TextStream::Flush()
	{
		Handle.WriteText(Buffer);
		Buffer.Clear();
	}

	StringView TextStream::Read()
	{
		StringView Substring = Buffer.Substring(Cursor, Buffer.GetCount() - Cursor);
		Substring = StringUtility::Split(Substring, StringUtility::NewLine);
		Cursor = Math::Min(Cursor + Substring.GetCount() + 1, Buffer.GetCount());
		return Substring;
	}

	void TextStream::Write(StringView Data)
	{
		Buffer.Append(Data);
		Buffer.Append(StringUtility::NewLine);
	}

	BinaryStream::BinaryStream(StringView Path)
		: Stream(Path), Buffer(BlockSize), Cursor()
	{
	}

	BinaryStream::~BinaryStream()
	{
	}

	bool BinaryStream::IsAtTheEnd()
	{
		return Cursor == Buffer.GetCount();
	}

	void BinaryStream::Cache()
	{
		Buffer = Handle.ReadByte();
		Cursor = 0;
	}

	void BinaryStream::Flush()
	{
		Handle.WriteByte(Buffer);
		Buffer.Clear();
	}

	BufferView BinaryStream::Read(uint64 Size)
	{
		BufferView View = Buffer.Get(Size, Cursor);
		Cursor += Size;
		return View;
	}

	void BinaryStream::Write(BufferView Data)
	{
		if (Cursor + Data.GetCount() >= Buffer.GetCount())
		{
			uint64 Size = Math::Max(BlockSize, Data.GetCount());
			Buffer.Grow(Size);
		}

		Buffer.Set(Data.GetPtr(), Data.GetCount(), Cursor);
		Cursor += Data.GetCount();
	}
}


