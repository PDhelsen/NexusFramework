#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	RBS::OffsetScope::OffsetScope(RBS& Rbs, uint64 Offset)
		: Rbs(Rbs), Cursor(Rbs.GetCursor())
	{
		Rbs.SetCursor(Offset);
	}

	RBS::OffsetScope::~OffsetScope()
	{
		Rbs.SetCursor(Cursor);
	}

	Buffer RBS::Serialize(const RBS& Data)
	{
		return Data.Data;
	}

	void RBS::SerializeFile(const RBS& Data, StringView Path)
	{
		BinaryStream Stream(Path);
		Stream.Open(File::Mode::Write);
		Stream.WriteAll(Serialize(Data));
		Stream.Close();
	}

	RBS RBS::Deserialize(BufferView Data)
	{
		return RBS(Data);
	}

	RBS RBS::DeserializeFile(StringView Path)
	{
		BinaryStream Stream(Path);
		Stream.Open(File::Mode::Read);
		BufferView Data = Stream.ReadAll();
		Stream.Close();

		return Deserialize(Data);
	}

	RBS::RBS()
		: Data(1024), Cursor(0)
	{
	}

	RBS::RBS(BufferView View)
		: Data(View.GetCount()), Cursor(0)
	{
		Data.Set(View.GetPtr());
	}

	RBS::RBS(Buffer&& Data)
		: Data(Move(Data)), Cursor(0)
	{
	}

	const void* RBS::ReadByte(uint64 Size)
	{
		void* Ptr = Data.GetPtr(Cursor);
		Cursor += Size;
		return Ptr;
	}

	const void* RBS::ReadByte(uint64 Size, uint64 Offset)
	{
		OffsetScope CursorOffset(*this, Offset);
		return ReadByte(Size);
	}

	void RBS::WriteByte(const void* Pointer, uint64 Size)
	{
		Data.Set(Pointer, Size, Cursor);
		Cursor += Size;
	}

	void RBS::WriteByte(const void* Pointer, uint64 Size, uint64 Offset)
	{
		OffsetScope CursorOffset(*this, Offset);
		WriteByte(Pointer, Size);
	}

	const Iterator::IteratorPointer RBS::Begin() const
	{
		return Data.Begin();
	}

	const Iterator::IteratorPointer RBS::End() const
	{
		return Data.End();
	}
}
